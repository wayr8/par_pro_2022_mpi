// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <vector>
#include "./simpson.h"
#include <gtest-mpi-listener.hpp>

TEST(Test_simpson_method, test) {
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double x, double y, double z) {
        return 10 * x * y * z * z;
    };

    double result;

    double a[3] = {0, 0, 0};
    double b[3] = {1, 1, 1};
    double n[3] = {100, 100, 100};

    if (rank == 0) {
        result = integrateSequential(func, a, b, n);
        std::cout << result;
    }

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
