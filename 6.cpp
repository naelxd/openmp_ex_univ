#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <chrono>

// Имитация нагрузки: расчетная функция с неравномерным временем выполнения
void workload(int iteration, double& result) {
    std::mt19937 rng(iteration); // Генератор случайных чисел
    std::uniform_int_distribution<int> dist(1, 1000);
    int delay = dist(rng); // Неравномерная нагрузка

    result = 0;
    for (int i = 0; i < delay; ++i) {
        result += std::sin(i) * std::cos(i); // Некоторая вычислительная нагрузка
    }
}

int main() {
    const int num_iterations = 100000;
    std::vector<double> results(num_iterations, 0.0);


    int num_threads;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;
    omp_set_num_threads(num_threads);

    std::vector<std::string> schedules = {"static", "dynamic", "guided"};

    for (const auto& schedule : schedules) {
        // Установка типа планирования через omp_set_schedule
        omp_set_schedule(schedule == "static" ? omp_sched_static :
                     schedule == "dynamic" ? omp_sched_dynamic :
                     omp_sched_guided, 0);

        std::cout << "Schedule: " << schedule << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        #pragma omp parallel for schedule(runtime)
        for (int i = 0; i < num_iterations; ++i) {
            workload(i, results[i]);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    }

    return 0;
}

