#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

// Функция для генерации случайных векторов
std::vector<std::pair<std::vector<double>, std::vector<double>>> generate_random_vectors(size_t num_pairs, size_t vector_size) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>> vector_pairs;
    std::srand(static_cast<unsigned>(std::time(0)));

    for (size_t i = 0; i < num_pairs; ++i) {
        std::vector<double> vec1(vector_size), vec2(vector_size);
        for (size_t j = 0; j < vector_size; ++j) {
            vec1[j] = static_cast<double>(std::rand()) / RAND_MAX;
            vec2[j] = static_cast<double>(std::rand()) / RAND_MAX;
        }
        vector_pairs.emplace_back(vec1, vec2);
    }

    return vector_pairs;
}

// Последовательное вычисление скалярного произведения
std::vector<double> compute_sequential(const std::vector<std::pair<std::vector<double>, std::vector<double>>> &vector_pairs) {
    std::vector<double> results;
    for (const auto &[vec1, vec2] : vector_pairs) {
        double dot_product = 0.0;
        for (size_t i = 0; i < vec1.size(); ++i) {
            dot_product += vec1[i] * vec2[i];
        }
        results.push_back(dot_product);
    }
    return results;
}

// Параллельное вычисление скалярного произведения с использованием OpenMP и директивы sections
std::vector<double> compute_parallel(const std::vector<std::pair<std::vector<double>, std::vector<double>>> &vector_pairs) {
    std::vector<double> results(vector_pairs.size());

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            #pragma omp parallel for
            for (size_t i = 0; i < vector_pairs.size(); ++i) {
                const auto &[vec1, vec2] = vector_pairs[i];
                double dot_product = 0.0;
                for (size_t j = 0; j < vec1.size(); ++j) {
                    dot_product += vec1[j] * vec2[j];
                }
                results[i] = dot_product;
            }
        }
    }

    return results;
}

int main() {
    try {
        // Генерация случайных данных
        size_t num_pairs = 100000;      // Количество пар векторов
        size_t vector_size = 1000;    // Размер каждого вектора
        auto vector_pairs = generate_random_vectors(num_pairs, vector_size);

        std::cout << "Vector Size:" << vector_size << '\n';
        std::cout << "Num Pairs:" << num_pairs << '\n';

        // Последовательное вычисление и замер времени
        auto start_seq = std::chrono::high_resolution_clock::now();
        auto sequential_results = compute_sequential(vector_pairs);
        auto end_seq = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_seq = end_seq - start_seq;

        // Параллельное вычисление и замер времени
        auto start_par = std::chrono::high_resolution_clock::now();
        auto parallel_results = compute_parallel(vector_pairs);
        auto end_par = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_par = end_par - start_par;

        // Вывод результатов
        std::cout << "Sequential computation time: " << duration_seq.count() << " seconds\n";
        std::cout << "Parallel computation time: " << duration_par.count() << " seconds\n";

        // Проверка корректности
        bool correct = true;
        for (size_t i = 0; i < sequential_results.size(); ++i) {
            if (std::abs(sequential_results[i] - parallel_results[i]) > 1e-6) {
                correct = false;
                break;
            }
        }

        if (correct) {
            std::cout << "Results are correct\n";
        } else {
            std::cout << "Results are incorrect\n";
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

