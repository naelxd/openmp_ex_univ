#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

// Функция для генерации случайного вектора
std::vector<double> generate_vector(size_t vector_size) {
    std::vector<double> vec(vector_size);
    for (size_t j = 0; j < vector_size; ++j) {
        vec[j] = static_cast<double>(std::rand()) / RAND_MAX;
    }
    return vec;
}

// Функция для вычисления скалярного произведения
double dot_product(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double dot_product = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dot_product += vec1[i] * vec2[i];
    }
    return dot_product;
}

int main() {
    try {
        size_t num_pairs = 10000;      // Количество пар векторов
        size_t vector_size = 10000;    // Размер каждого вектора
        std::vector<std::vector<double>> vectors1(num_pairs);
        std::vector<std::vector<double>> vectors2(num_pairs);
        std::vector<double> results(num_pairs);

        // Измерение времени параллельного алгоритма
        auto start_parallel = std::chrono::high_resolution_clock::now();

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                // Генерация векторов
                for (size_t i = 0; i < num_pairs; ++i) {
                    vectors1[i] = generate_vector(vector_size);
                    vectors2[i] = generate_vector(vector_size);
                }
            }

            #pragma omp section
            {
                // Вычисление скалярного произведения
                for (size_t i = 0; i < num_pairs; ++i) {
                    while (vectors1[i].empty() || vectors2[i].empty()) {
                    }

                    results[i] = dot_product(vectors1[i], vectors2[i]);
                }
            }
        }

        auto end_parallel = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_parallel = end_parallel - start_parallel;
        std::cout << "Parallel computation time: " << duration_parallel.count() << " seconds\n";

        // Измерение времени последовательного алгоритма
        auto start_sequential = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < num_pairs; ++i) {
            vectors1[i] = generate_random_vector(vector_size);
            vectors2[i] = generate_random_vector(vector_size);
            results[i] = compute_dot_product(vectors1[i], vectors2[i]);
        }

        auto end_sequential = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_sequential = end_sequential - start_sequential;
        std::cout << "Sequential computation time: " << duration_sequential.count() << " seconds\n";

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

