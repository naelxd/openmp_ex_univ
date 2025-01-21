#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

// Функция для интегрирования
double f(double x) {
    return x*x*x;
}

// Последовательный метод прямоугольников
double sequential_integral(double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        integral += f(x) * h;
    }

    return integral;
}

// Параллельный метод прямоугольников с использованием OpenMP
double parallel_integral(double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.0;

    #pragma omp parallel for reduction(+:integral)
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        integral += f(x) * h;
    }

    return integral;
}

int main() {
    double a = 0.0;  // Нижний предел интегрирования
    double b = 1000000; // Верхний предел интегрирования
    int n = 10000000; // Количество шагов

    // Последовательный метод
    auto start_seq = std::chrono::high_resolution_clock::now();
    double result_seq = sequential_integral(a, b, n);
    auto end_seq = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_seq = end_seq - start_seq;

    // Параллельный метод
    auto start_par = std::chrono::high_resolution_clock::now();
    double result_par = parallel_integral(a, b, n);
    auto end_par = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_par = end_par - start_par;

    // Вывод результатов
    std::cout << "Последовательный метод: результат = " << result_seq
              << ", время = " << duration_seq.count() << " секунд\n";
    std::cout << "Параллельный метод: результат = " << result_par
              << ", время = " << duration_par.count() << " секунд\n";

    return 0;
}

