#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#define ROWS 10000 // Число строк матрицы
#define BANDWIDTH 10 // Ширина ленты (ненулевые элементы)

// Функция для инициализации ленточной матрицы
std::vector<std::vector<int>> initialize_band_matrix() {
    std::vector<std::vector<int>> matrix(ROWS, std::vector<int>(ROWS, 0));
    for (int i = 0; i < ROWS; i++) {
        for (int j = std::max(0, i - BANDWIDTH); j <= std::min(ROWS - 1, i + BANDWIDTH); j++) {
            matrix[i][j] = rand() % 100;
        }
    }
    return matrix;
}


// Функция для инициализации треугольной матрицы
std::vector<std::vector<int>> initialize_triangular_matrix() {
    std::vector<std::vector<int>> matrix(ROWS, std::vector<int>(ROWS, 0));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= i; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
    return matrix;
}

// Последовательный метод поиска максимального среди минимальных элементов строк
int find_max_of_min_sequential(const std::vector<std::vector<int>> &matrix) {
    int max_min = -1;

    for (int i = 0; i < ROWS; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < ROWS; ++j) {
            if (matrix[i][j] < min_in_row) {
                min_in_row = matrix[i][j];
            }
        }
        if (min_in_row > max_min) {
            max_min = min_in_row;
        }
    }

    return max_min;
}

// Параллельный метод с использованием OpenMP и редукции
int find_max_of_min_parallel(const std::vector<std::vector<int>> &matrix) {
    int max_min = -1;

    #pragma omp parallel for schedule(runtime) reduction(max:max_min)
    for (int i = 0; i < ROWS; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < ROWS; ++j) {
            if (matrix[i][j] < min_in_row) {
                min_in_row = matrix[i][j];
            }
        }
        if (min_in_row > max_min) {
            max_min = min_in_row;
        }
    }

    return max_min;
}


int main() {
    // Инициализация случайного генератора чисел
    srand(static_cast<unsigned>(time(0)));

    // Создание ленточной матрицы
    std::vector<std::vector<int>> band_matrix = initialize_band_matrix();

    // Создание треугольной матрицы
    std::vector<std::vector<int>> triangular_matrix = initialize_triangular_matrix();

    // Последовательное выполнение для поиска максимального среди минимальных элементов (ленточная матрица)
    double start_time = omp_get_wtime();
    int max_min_band = find_max_of_min_sequential(band_matrix);
    double end_time = omp_get_wtime();
    std::cout << "Последовательный метод (ленточная матрица): максимум среди минимумов = " << max_min_band << ", время выполнения = " << (end_time - start_time) << " секунд\n";

    // Последовательное выполнение для поиска максимального среди минимальных элементов (треугольная матрица)
    start_time = omp_get_wtime();
    int max_min_triangular = find_max_of_min_sequential(triangular_matrix);
    end_time = omp_get_wtime();
    std::cout << "Последовательный метод (треугольная матрица): максимум среди минимумов = " << max_min_triangular << ", время выполнения = " << (end_time - start_time) << " секунд\n";

     // Параллельное выполнение с разными правилами распределения для ленточной и треугольной матриц
    std::vector<std::string> schedules = {"static", "dynamic", "guided"};
    for (const auto &schedule : schedules) {
        omp_set_schedule(schedule == "static" ? omp_sched_static :
                     schedule == "dynamic" ? omp_sched_dynamic :
                     omp_sched_guided, 0);
        std::cout << "-----" << '\n';
        std::cout << schedule << '\n';
        start_time = omp_get_wtime();
        int max_min_band = find_max_of_min_parallel(band_matrix);
        end_time = omp_get_wtime();
        std::cout << "Параллельный метод (ленточная матрица): максимум среди минимумов = " << max_min_band << ", время выполнения = " << (end_time - start_time) << " секунд\n";

        start_time = omp_get_wtime();
        int max_min_triangular = find_max_of_min_parallel(triangular_matrix);
        end_time = omp_get_wtime();
        std::cout << "Параллельный метод (треугольная матрица): максимум среди минимумов = " << max_min_triangular << ", время выполнения = " << (end_time - start_time) << " секунд\n";
    }
}
