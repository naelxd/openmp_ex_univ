#include <iostream>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        // Выводим количество потоков в параллельной области
        #pragma omp single
        std::cout << "Number of threads in parallel region: " << omp_get_num_threads() << std::endl;
    }

    // Выводим максимальное количество потоков
    std::cout << "Default maximum threads: " << omp_get_max_threads() << std::endl;

    return 0;
}

