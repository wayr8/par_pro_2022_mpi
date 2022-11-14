// Copyright 2022 Nikolaev Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/nikolaev_a_vector_average/vector_average.h"

#define FIRST_THREAD 0

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int  i = 0; i < size; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getAverageVectorSequential(std::vector<int> vec) {
    const int size = vec.size();
    int sum = 0;
    
    for(int i=0; i<size; i++) {
        sum += vec[i];
    }

    return sum/size;
}

int getAverageVectorParallel(std::vector<int> vec, int count_size_vector) {
    int size, rank, sum=1, n, i, ibeg, iend;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int SizeVector = count_size_vector;

    n = (SizeVector - 1) / size + 1;
    ibeg = rank*n;
    iend = (rank + 1) * n;

    if(rank != FIRST_THREAD) {
        MPI_Status status;
        MPI_Recv(&sum, 1, MPI_INT, rank-1, 10, MPI_COMM_WORLD, &status);
    }

    for(i=ibeg; i<((iend>SizeVector)?SizeVector:iend); i++) {
      sum = sum + vec[i];
    }

    if(rank != size-1) {
        MPI_Send(&sum, 1, MPI_INT, rank + 1, 10, MPI_COMM_WORLD);
    }

    // int size, rank;
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // const int delta = count_size_vector() / size;

    // if (rank == 0) {
    //     for (int proc = 1; proc < size; proc++) {
    //         MPI_Send(vec.data() + proc * delta, delta,
    //                     MPI_INT, proc, 0, MPI_COMM_WORLD);
    //     }
    // }

    // std::vector<int> local_vec(delta);
    // if (rank == 0) {
    //     local_vec = std::vector<int>(vec.begin(),
    //                                  vec.begin() + delta);
    // } else {
    //     MPI_Status status;
    //     MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // }

    // int global_sum = 0;
    // int local_average = getAverageVectorSequential(local_vec);
    // MPI_Reduce(&local_average, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // return global_sum/count_size_vector;
    return sum / SizeVector;
}
