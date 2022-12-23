#include <iostream>
#include <mpi.h>
#include <random>
#include "./vector_max.h"


void printVector(std::vector<int> vec) {
    for (int i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";
    std::cout << std::endl;
}

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

int getMax(std::vector<int> vec) {
    int max = vec[0];
    for (int i = 1; i < vec.size(); ++i)
        if (vec[i] > max)
            max = vec[i];
    return max;
}

int getMaxParallel(std::vector<int> vec, int vec_size) {
    int rank, comm_size;

    int total_max = 0, proc_max = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Bcast(vec.data(), vec_size, MPI_INT, 0, MPI_COMM_WORLD);

    int delta = vec_size / comm_size;
    int i1 = delta * rank;
    int i2 = delta * (rank + 1);

    if (rank == comm_size - 1) 
        i2 = vec_size;

    for (int i = i1; i < i2; ++i) {
        if (vec[i] > proc_max) {
            proc_max = vec[i];
        }
    }

    std::cout << "vec_size = " << vec_size << "\n";
    std::cout << "rank = " << rank << "\n";
    std::cout << "comm_size = " << comm_size << "\n";
    std::cout << "delta = " << delta << "\n";
    std::cout << "i1 = " << i1 << "\n";
    std::cout << "i2 = " << i2 << "\n";
    std::cout << "proc_max = " << proc_max << "\n";
    std::cout << "total_max = " << total_max << "\n";

    MPI_Reduce(&proc_max, &total_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return total_max;

}




