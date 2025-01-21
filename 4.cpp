#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#define ROWS 100 // Размер матрицы (строки)
#define COLS 10000 // Размер матрицы (столбцы)

// Функция для инициализации матрицы случайными числами
void initialize_matrix(std::vector<std::vector<int>> &matrix) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            matrix[i][j] = rand() % 100; // Случайное число от 0 до 99
        }
    }
}

// Последовательный метод
int find_max_of_min_sequential(const std::vector<std::vector<int>> &matrix) {
    int max_min = -1;

    for (int i = 0; i < ROWS; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < COLS; ++j) {
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

    #pragma omp parallel for reduction(max:max_min)
    for (int i = 0; i < ROWS; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < COLS; ++j) {
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

    // Создание и заполнение матрицы
    std::vector<std::vector<int>> matrix(ROWS, std::vector<int>(COLS));
    initialize_matrix(matrix);

    // Последовательное выполнение
    double start_time = omp_get_wtime();
    int max_min_seq = find_max_of_min_sequential(matrix);
    double end_time = omp_get_wtime();
    std::cout << "Последовательный метод: Максимум среди минимумов = " << max_min_seq
              << ", время выполнения = " << (end_time - start_time) << " секунд\n";

    // Параллельное выполнение
    start_time = omp_get_wtime();
    int max_min_par = find_max_of_min_parallel(matrix);
    end_time = omp_get_wtime();
    std::cout << "Параллельный метод: Максимум среди минимумов = " << max_min_par
              << ", время выполнения = " << (end_time - start_time) << " секунд\n";

    return 0;
}

