// Copyright 2022 Nikolaev Alexander
#include <iostream>
#include <mpi.h>
#include <vector>
#include <string>
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

int getAverageVectorParallel(std::vector<int> vec) {
    int size, rank, sum=0, n, i, ibeg, iend;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int SizeVector = vec.size();

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

    return sum/SizeVector;
}
