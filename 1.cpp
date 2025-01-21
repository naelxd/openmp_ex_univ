#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <cstdlib>
#include <ctime>

// Функция для создания случайного вектора
std::vector<int> generate_random_vector(int n, int min_val, int max_val) {
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = min_val + rand() % (max_val - min_val + 1);
    }
    return vec;
}

// Функция для нахождения минимума и максимума без OpenMP
std::pair<int, int> find_min_max(const std::vector<int>& vec) {
    int min_val = std::numeric_limits<int>::max();
    int max_val = std::numeric_limits<int>::min();
    for (int val : vec) {
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    return {min_val, max_val};
}

// Функция для нахождения минимума и максимума с OpenMP (с редукцией)
std::pair<int, int> find_min_max_with_reduction(const std::vector<int>& vec) {
    int min_val = std::numeric_limits<int>::max();
    int max_val = std::numeric_limits<int>::min();

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] < min_val) min_val = vec[i];
        if (vec[i] > max_val) max_val = vec[i];
    }

    return {min_val, max_val};
}

// Функция для нахождения минимума и максимума с OpenMP (без редукции)
std::pair<int, int> find_min_max_without_reduction(const std::vector<int>& vec) {
    int min_val = std::numeric_limits<int>::max();
    int max_val = std::numeric_limits<int>::min();

    #pragma omp parallel
    {
        int local_min = std::numeric_limits<int>::max();
        int local_max = std::numeric_limits<int>::min();

        #pragma omp for
        for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] < local_min) local_min = vec[i];
            if (vec[i] > local_max) local_max = vec[i];
        }

        #pragma omp critical
        {
            if (local_min < min_val) min_val = local_min;
            if (local_max > max_val) max_val = local_max;
        }
    }

    return {min_val, max_val};
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    int n = 5000; // Размер вектора
    int min_random = -50, max_random = 50; // Диапазон случайных чисел

    // Создаем случайный вектор
    std::vector<int> vec = generate_random_vector(n, min_random, max_random);

    // Выводим сгенерированный вектор
    std::cout << "Generated vector: ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Нахождение минимума и максимума без OpenMP
    double start_time = omp_get_wtime();
    auto [min_val, max_val] = find_min_max(vec);
    double end_time = omp_get_wtime();
    std::cout << "Minimum (no OpenMP): " << min_val << std::endl;
    std::cout << "Maximum (no OpenMP): " << max_val << std::endl;
    std::cout << "Execution time (no OpenMP): " << (end_time - start_time) << " seconds" << std::endl;

    // Нахождение минимума и максимума с редукцией
    start_time = omp_get_wtime();
    auto [min_val_reduction, max_val_reduction] = find_min_max_with_reduction(vec);
    end_time = omp_get_wtime();
    std::cout << "Minimum (with reduction): " << min_val_reduction << std::endl;
    std::cout << "Maximum (with reduction): " << max_val_reduction << std::endl;
    std::cout << "Execution time (with reduction): " << (end_time - start_time) << " seconds" << std::endl;

    // Нахождение минимума и максимума без редукции
    start_time = omp_get_wtime();
    auto [min_val_no_reduction, max_val_no_reduction] = find_min_max_without_reduction(vec);
    end_time = omp_get_wtime();
    std::cout << "Minimum (without reduction): " << min_val_no_reduction << std::endl;
    std::cout << "Maximum (without reduction): " << max_val_no_reduction << std::endl;
    std::cout << "Execution time (without reduction): " << (end_time - start_time) << " seconds" << std::endl;

    return 0;
}

