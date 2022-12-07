  // Copyright 2022 Ermolaev Danila
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <iostream>
#include "./val_rows_matrix_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Gauss_MPI, Random_Matrix_test_5x5) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 5;
    int rank;
    int col_proc;
    int* vec = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);

    double* matrix = nullptr;
    double* result = nullptr;
    double* matrixB = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz)];
        matrixB = new double[sz * (sz + 1)];
        matrix = getRandomMatrix(sz);

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 6 + 1;
        }

        matrixB = getRandomMatrixWithBvector(vec, matrix, sz);
    }

    result = getParallelGausJordan(matrixB, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            ASSERT_NEAR(vec[i], result[i], 0.001);
        }
    }

    delete[] vec;
    delete[] matrix;
    delete[] result;
}
TEST(Parallel_Gauss_MPI, Random_Matrix_test_20x20) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 20;
    int rank;
    int col_proc;
    int* vec = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);

    double* matrix = nullptr;
    double* result = nullptr;
    double* matrixB = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz)];
        matrixB = new double[sz * (sz + 1)];
        matrix = getRandomMatrix(sz);

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 6 + 1;
        }

        matrixB = getRandomMatrixWithBvector(vec, matrix, sz);
    }

    result = getParallelGausJordan(matrixB, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            ASSERT_NEAR(vec[i], result[i], 0.001);
        }
    }

    delete[] vec;
    delete[] matrix;
    delete[] result;
}
TEST(Parallel_Gauss_MPI, Random_Matrix_test_100x100) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 100;
    int rank;
    int col_proc;
    int* vec = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);

    double* matrix = nullptr;
    double* result = nullptr;
    double* matrixB = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz)];
        matrixB = new double[sz * (sz + 1)];
        matrix = getRandomMatrix(sz);

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 6 + 1;
        }
        matrixB = getRandomMatrixWithBvector(vec, matrix, sz);
    }
    result = getParallelGausJordan(matrixB, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            ASSERT_NEAR(vec[i], result[i], 0.001);
        }
    }

    delete[] vec;
    delete[] matrix;
    delete[] result;
}
TEST(Parallel_Gauss_MPI, Random_Matrix_test_1000x1000) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 1000;
    int rank;
    int col_proc;
    int* vec = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);

    double* matrix = nullptr;
    double* result = nullptr;
    double* matrixB = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz)];
        matrixB = new double[sz * (sz + 1)];
        matrix = getRandomMatrix(sz);

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 6 + 1;
        }

        matrixB = getRandomMatrixWithBvector(vec, matrix, sz);
    }

    result = getParallelGausJordan(matrixB, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            ASSERT_NEAR(vec[i], result[i], 0.001);
        }
    }

    delete[] vec;
    delete[] matrix;
    delete[] result;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
