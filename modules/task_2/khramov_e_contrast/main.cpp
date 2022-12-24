// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include "../../../modules/task_2/khramov_e_contrast/contrast.h"
#include <gtest-mpi-listener.hpp>


TEST(Test_contrast, Test_0) {
    int rank;

    int w = 5, h = 4;

    std::vector<int> matrix;
    std::vector<int> resultSequential;
    std::vector<int> resultParallel;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {

        // std::cout << 5 / 2;

        matrix = getRandomMatrix(w, h);

        std::cout << "Main vector: "; printVector(matrix);
        std::cout << std::endl;
        // resultSequential = getContrastedMatrixSequential(matrix);

        // std::cout << "First matrix: " << std::endl;
        // printVector(matrix);

        // std::cout << "Result matrix: " << std::endl;
        // printVector(resultSequential);
    }

    resultParallel = getContrastedMatrixParallel(matrix, w * h);

    ASSERT_EQ(1, 1);
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
