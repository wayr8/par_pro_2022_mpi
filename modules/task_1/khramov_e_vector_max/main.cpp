// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <vector>
#include "./vector_max.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Test_Vector_Max_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector = getRandomVector(20);

    printVector(randomVector);
    
    ASSERT_EQ(2, 2);

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