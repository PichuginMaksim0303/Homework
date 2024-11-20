#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    // Количество элементов в наборе K + 5
    const int K = ProcNum;
    const int N = K + 5;

    // Инициализируем генератор случайных чисел
    std::srand(std::time(0) + ProcRank);
    double start_time = MPI_Wtime();
    // В каждом процессе создаем набор из N целых чисел
    std::vector<int> data(N);
    for (int i = 0; i < N; ++i) {
        data[i] = std::rand() % 100;  // Заполняем случайными числами
    }

    // Структура для хранения максимума и ранга процесса
    struct {
        int value;  // Максимальное значение
        int rank;   // Ранг процесса, где находится это значение
    } local_max, global_max;

    // Главный процесс (процесс 0) будет собирать результаты
    std::vector<int> max_values(N);    // Максимальные значения для каждого элемента
    std::vector<int> max_ranks(N);     // Ранги процессов, где находятся максимальные значения

    for (int i = 0; i < N; ++i) {
        // Для каждого элемента данных из набора находим максимум и процесс
        local_max.value = data[i];
        local_max.rank = ProcRank;

        // Применяем MPI_Reduce с операцией MPI_MAXLOC, чтобы найти максимум
        MPI_Reduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

        // Главный процесс сохраняет результаты
        if (ProcRank == 0) {
            max_values[i] = global_max.value;
            max_ranks[i] = global_max.rank;
        }
    }

    // Главный процесс выводит результаты
    if (ProcRank == 0) {
        std::cout << "Максимальные значения: ";
        for (int i = 0; i < N; ++i) {
            std::cout << max_values[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Ранги процессов с максимальными значениями: ";
        for (int i = 0; i < N; ++i) {
            std::cout << max_ranks[i] << " ";
        }
        std::cout << std::endl;
    }
    // Замер времени завершения
    double end_time = MPI_Wtime();
    // Вывод времени выполнения программы каждым процессом
    if (ProcRank == 0) {
        std::cout << "Execution time: " << (end_time - start_time) << " seconds" << std::endl;
    }
    MPI_Finalize();
    return 0;
}