#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <limits>
#include <ctime>

// Функция для генерации случайной матрицы
std::vector<std::vector<int>> generateMatrix(int rows, int cols, int minVal = 1, int maxVal = 100) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = minVal + rand() % (maxVal - minVal + 1);
        }
    }
    return matrix;
}

// Последовательный алгоритм
int sequentialMaxOfMins(const std::vector<std::vector<int>> &matrix, int rows, int cols) {
    int max_min = -1;

    for (int i = 0; i < rows; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < cols; ++j) {
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

// Параллельный алгоритм без вложенного параллелизма
int parallelMaxOfMins(const std::vector<std::vector<int>> &matrix, int rows, int cols) {
    int max_min = -1;

    #pragma omp parallel for reduction(max:max_min)
    for (int i = 0; i < rows; ++i) {
        int min_in_row = matrix[i][0];
        for (int j = 1; j < cols; ++j) {
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

// Параллельный алгоритм с вложенным параллелизмом
int nestedParallelMaxOfMins(const std::vector<std::vector<int>> &matrix) {
    int maxMin = std::numeric_limits<int>::min();
    omp_set_nested(1); // Включение вложенного параллелизма

    #pragma omp parallel for reduction(max : maxMin)
    for (int i = 0; i < matrix.size(); ++i) {
        int rowMin = std::numeric_limits<int>::max();

        #pragma omp parallel for reduction(min : rowMin)
        for (int j = 0; j < matrix[i].size(); ++j) {
            rowMin = std::min(rowMin, matrix[i][j]);
        }

        maxMin = std::max(maxMin, rowMin);
    }
    return maxMin;
}

int main() {
    // Параметры матрицы
    const int rows = 10;
    const int cols = 10;

    std::cout << "rows: " << rows << '\n';
    std::cout << "cols: " << cols << '\n';

    // Генерация матрицы
    std::srand(std::time(0));
    std::vector<std::vector<int>> matrix = generateMatrix(rows, cols);

    // Последовательный алгоритм
    double start = omp_get_wtime();
    int seqResult = sequentialMaxOfMins(matrix, rows, cols);
    double seqTime = omp_get_wtime() - start;

    // Параллельный алгоритм без вложенного параллелизма
    start = omp_get_wtime();
    int parResult = parallelMaxOfMins(matrix, rows, cols);
    double parTime = omp_get_wtime() - start;

    // Параллельный алгоритм с вложенным параллелизмом
    start = omp_get_wtime();
    int nestedParResult = nestedParallelMaxOfMins(matrix);
    double nestedParTime = omp_get_wtime() - start;

    // Вывод результатов
    std::cout << "Sequential result: " << seqResult << ", Time: " << seqTime << " seconds" << std::endl;
    std::cout << "Parallel result (no nesting): " << parResult << ", Time: " << parTime << " seconds" << std::endl;
    std::cout << "Parallel result (with nesting): " << nestedParResult << ", Time: " << nestedParTime << " seconds" << std::endl;

    return 0;
}

