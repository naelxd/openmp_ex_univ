#include <iostream>
#include <vector>
#include <omp.h>
#include <numeric>
#include <chrono>

// Инициализация большого массива
void initialize_array(std::vector<int> &array, int value = 1) {
    std::fill(array.begin(), array.end(), value);
}

int main() {
    const size_t SIZE = 10000000; // Размер массива
    std::vector<int> array(SIZE);
    initialize_array(array);

    int num_threads;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;
    omp_set_num_threads(num_threads);

    std::cout << "SIZE: " << SIZE << '\n';

    // Последовательная редукция
    std::cout << "Sequential Reduction:\n";
    long long sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < SIZE; ++i) {
        sum += array[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";

    // Редукция с помощью атомарных операций
    std::cout << "Atomic Reduction:\n";
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    #pragma omp parallel for
    for (size_t i = 0; i < SIZE; ++i) {
        #pragma omp atomic
        sum += array[i];
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";

    // Редукция с помощью критической секции
    std::cout << "Critical Section Reduction:\n";
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    #pragma omp parallel for
    for (size_t i = 0; i < SIZE; ++i) {
        #pragma omp critical
        {
            sum += array[i];
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";

    // Редукция с использованием замков
    std::cout << "Lock-based Reduction:\n";
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    #pragma omp parallel for
    for (size_t i = 0; i < SIZE; ++i) {
        omp_set_lock(&lock);
        sum += array[i];
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";

    // Редукция с использованием директивы reduction
    std::cout << "OpenMP Reduction Directive:\n";
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < SIZE; ++i) {
        sum += array[i];
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";

    return 0;
}

