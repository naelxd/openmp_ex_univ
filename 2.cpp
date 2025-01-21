#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

// Функция для генерации случайного вектора заданной длины
std::vector<int> generateRandomVector(size_t size) {
    std::vector<int> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = rand() % 100; // случайные числа от 0 до 99
    }
    return vec;
}

// Последовательное вычисление скалярного произведения
long long dotProductSequential(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    long long result = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += static_cast<long long>(vec1[i]) * vec2[i];
    }
    return result;
}

// Параллельное вычисление скалярного произведения с использованием OpenMP
long long dotProductParallel(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    long long result = 0;

    #pragma omp parallel for reduction(+:result)
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += static_cast<long long>(vec1[i]) * vec2[i];
    }

    return result;
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(0)));

    // Размер векторов
    size_t vectorSize = 100000000;

    // Генерация случайных векторов
    std::vector<int> vec1 = generateRandomVector(vectorSize);
    std::vector<int> vec2 = generateRandomVector(vectorSize);

    // Последовательное вычисление
    double startTime = omp_get_wtime();
    long long sequentialResult = dotProductSequential(vec1, vec2);
    double sequentialTime = omp_get_wtime() - startTime;

    // Параллельное вычисление
    startTime = omp_get_wtime();
    long long parallelResult = dotProductParallel(vec1, vec2);
    double parallelTime = omp_get_wtime() - startTime;

    // Вывод результатов
    std::cout << "Sequential Result: " << sequentialResult << std::endl;
    std::cout << "Sequential Time: " << sequentialTime << " seconds" << std::endl;

    std::cout << "Parallel Result: " << parallelResult << std::endl;
    std::cout << "Parallel Time: " << parallelTime << " seconds" << std::endl;

    return 0;
}

