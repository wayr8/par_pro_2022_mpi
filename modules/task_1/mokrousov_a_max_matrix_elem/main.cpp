// Copyright 2022 Mokrousov Artem
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./max_matrix_elem.h"

TEST(Parallel_Operations_MPI, Test_Sqrt_matr) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int** matr = nullptr;
    int n = 10;
    int m = 10;

    if (rank == 0) {
        matr = getRandomMatrix(n, m);
    }

    int global_max;
    global_max = parallelMaximum(matr, n, m);

    if (rank == 0) {
        int reference_max = getSequentialMaximum(matr, n, m);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_20_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int** matr = nullptr;
    int n = 20;
    int m = 10;

    if (rank == 0) {
        matr = getRandomMatrix(n, m);
    }

    int global_max;
    global_max = parallelMaximum(matr, n, m);

    if (rank == 0) {
        int reference_max = getSequentialMaximum(matr, n, m);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_10_20) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int** matr = nullptr;
    int n = 10;
    int m = 20;

    if (rank == 0) {
        matr = getRandomMatrix(n, m);
    }

    int global_max;
    global_max = parallelMaximum(matr, n, m);

    if (rank == 0) {
        int reference_max = getSequentialMaximum(matr, n, m);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_2_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int** matr = nullptr;
    int n = 2;
    int m = 1;

    if (rank == 0) {
        matr = getRandomMatrix(n, m);
    }

    int global_max;
    global_max = parallelMaximum(matr, n, m);

    if (rank == 0) {
        int reference_max = getSequentialMaximum(matr, n, m);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_100_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int** matr = nullptr;
    int n = 100;
    int m = 100;

    if (rank == 0) {
        matr = getRandomMatrix(n, m);
    }

    int global_max;
    global_max = parallelMaximum(matr, n, m);

    if (rank == 0) {
        int reference_max = getSequentialMaximum(matr, n, m);
        ASSERT_EQ(reference_max, global_max);
    }
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
