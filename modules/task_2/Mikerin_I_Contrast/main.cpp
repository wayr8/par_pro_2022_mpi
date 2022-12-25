// Copyright 2022 Mikerin Ilya
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_2/Mikerin_I_Contrast/Contrast.h"
#include <gtest-mpi-listener.hpp>

#define debug

TEST(Parallel, Concret) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_vec;
    int weight = 3;
    int height = 4;

    if (rank == 0)
        global_vec = std::vector<std::vector<int>>({{1, 0, 1, 1}, { 0, 1, 1, 1}, { 0, 1, 1, 1}});

    res_vec = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_vec, std::vector<std::vector<int>>(
            {{255, 50, 255, 255}, { 50, 255, 255, 255}, { 50, 255, 255, 255}}));
    }
}

TEST(Parallel, RANDOM) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_paral;
    std::vector<std::vector<int>> res_seq;
    int weight = 5;
    int height = 7;

    if (rank == 0) {
        global_vec = std::vector<std::vector<int>>(weight);

        for (int i =0; i < weight; i++) {
            global_vec[i] = getRandomVector(height);
        }
        res_seq = SequentialContrast(global_vec, weight, height);
    }

    res_paral = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_paral, res_seq);
    }
}



TEST(Parallel, ONELINEFULL) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_paral;
    std::vector<std::vector<int>> res_seq;
    int weight = 1;
    int height = size * 20;

    if (rank == 0) {
        global_vec = std::vector<std::vector<int>>(weight);

        for (int i =0; i < weight; i++) {
            global_vec[i] = getRandomVector(height);
        }
        res_seq = SequentialContrast(global_vec, weight, height);
    }

    res_paral = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_paral, res_seq);
    }
}

TEST(Parallel, ONELINEONEREM) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_paral;
    std::vector<std::vector<int>> res_seq;
    int weight = 1;
    int height = size * 20 + 1;

    if (rank == 0) {
        global_vec = std::vector<std::vector<int>>(weight);

        for (int i =0; i < weight; i++) {
            global_vec[i] = getRandomVector(height);
        }
        res_seq = SequentialContrast(global_vec, weight, height);
    }

    res_paral = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_paral, res_seq);
    }
}

TEST(Parallel, ONELINEMOREONEREM) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_paral;
    std::vector<std::vector<int>> res_seq;
    int weight = 1;
    int height = size * 20 - 1;

    if (rank == 0) {
        global_vec = std::vector<std::vector<int>>(weight);

        for (int i =0; i < weight; i++) {
            global_vec[i] = getRandomVector(height);
        }
        res_seq = SequentialContrast(global_vec, weight, height);
    }

    res_paral = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_paral, res_seq);
    }
}

TEST(Parallel, ONECOL) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> global_vec;
    std::vector<std::vector<int>> res_paral;
    std::vector<std::vector<int>> res_seq;
    int weight = size * 20 - 1;
    int height = 1;

    if (rank == 0) {
        global_vec = std::vector<std::vector<int>>(weight);

        for (int i =0; i < weight; i++) {
            global_vec[i] = getRandomVector(height);
        }
        res_seq = SequentialContrast(global_vec, weight, height);
    }

    res_paral = ParallelContrast(global_vec, weight, height);


    if (rank == 0) {
        ASSERT_EQ(res_paral, res_seq);
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
