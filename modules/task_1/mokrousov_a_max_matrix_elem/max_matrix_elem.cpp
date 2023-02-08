// Copyright 2022 Mokrousov Artem
#include "../../../modules/task_1/mokrousov_a_max_matrix_elem/max_matrix_elem.h"

#include <mpi.h>

#include <iostream>
#include <random>

int** getRandomMatrix(int n, int m) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int** matr = new int*[n];
    for (int i = 0; i < n; i++) matr[i] = new int[m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matr[i][j] = gen() % 100;
        }
    }
    return matr;
}

int getSequentialMaximum(int** matr, int n, int m) {
    int max = matr[0][0];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            max = std::max(max, matr[i][j]);
        }
    }
    return max;
}

int parallelMaximum(int** matr, int n, int m) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int localMatrSize;
    if (rank == 0)
        localMatrSize = n / size + n % size;
    else
        localMatrSize = n / size;
    int** localMatrix = new int*[localMatrSize];
    for (int i = 0; i < localMatrSize; i++) {
        localMatrix[i] = new int[m];
    }
    if (rank == 0) {
        for (int i = 0; i < localMatrSize; i++) {
            localMatrix[i] = matr[i];
        }
        int sendSize = n / size;
        if (sendSize > 0) {
            for (int i = 1; i < size; i++) {
                for (int j = 0; j < sendSize; j++) {
                    MPI_Send(matr[localMatrSize + sendSize * (i - 1) + j], m,
                             MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
        }
    } else {
        for (int i = 0; i < localMatrSize; i++) {
            MPI_Status status;
            MPI_Recv(localMatrix[i], m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    int localRes;
    if (localMatrSize == 0 && rank != 0)
        localRes = 0;
    else
        localRes = getSequentialMaximum(localMatrix, localMatrSize, m);
    int globalRes = 0;
    MPI_Reduce(&localRes, &globalRes, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return globalRes;
}
