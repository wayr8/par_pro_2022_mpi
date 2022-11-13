// Copyright 2022 Nikolaev Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./vector_average.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Average_MPI, Test_Average1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 200;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_average = getAverageVectorParallel(global_vec);

    if (rank == 0) {
        int sequential_average = getAverageVectorSequential(global_vec);
        ASSERT_EQ(sequential_average, parallel_average);
    }
}

TEST(Vector_Average_MPI, Test_Average2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 2000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_average = getAverageVectorParallel(global_vec);

    if (rank == 0) {
        int sequential_average = getAverageVectorSequential(global_vec);
        ASSERT_EQ(sequential_average, parallel_average);
    }
}

TEST(Vector_Average_MPI, Test_Average3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 20000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_average = getAverageVectorParallel(global_vec);

    if (rank == 0) {
        int sequential_average = getAverageVectorSequential(global_vec);
        ASSERT_EQ(sequential_average, parallel_average);
    }
}

TEST(Vector_Average_MPI, Test_Average4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 200000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_average = getAverageVectorParallel(global_vec);

    if (rank == 0) {
        int sequential_average = getAverageVectorSequential(global_vec);
        ASSERT_EQ(sequential_average, parallel_average);
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
