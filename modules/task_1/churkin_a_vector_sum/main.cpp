// Copyright 2022 Churkin Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./vector_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Sum_MPI, Test_Vector_Sum_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 1000;

    std::vector<int> globalVector;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        int resSeq = getSumSequential(globalVector);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 999;

    std::vector<int> globalVector;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        int resSeq = getSumSequential(globalVector);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 998;

    std::vector<int> globalVector;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        int resSeq = getSumSequential(globalVector);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 997;

    std::vector<int> globalVector;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        int resSeq = getSumSequential(globalVector);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 100;

    std::vector<int> globalVector;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        int resSeq = getSumSequential(globalVector);
        ASSERT_EQ(resPar, resSeq);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
