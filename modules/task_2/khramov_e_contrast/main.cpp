// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include "../../../modules/task_2/khramov_e_contrast/contrast.h"
#include <gtest-mpi-listener.hpp>


TEST(Test_contrast, Test_0) {
    int size, rank;

    auto matrix = getRandomMatrix(10, 5);

    auto result = getContrastedMatrixSequential(matrix);

    std::cout << "First matrix: " << std::endl;
    printMatrix(matrix);

    std::cout << "Result matrix: " << std::endl;
    printMatrix(result);

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
