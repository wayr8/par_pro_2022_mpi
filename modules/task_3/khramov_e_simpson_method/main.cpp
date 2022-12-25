// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <vector>
#include "./simpson.h"
#include <gtest-mpi-listener.hpp>

const double epsilon = 0.01;

TEST(Test_simpson_method, test) {
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double x, double y, double z) {
        return 10 * x * y * z * z;
    };

    double resultSequential, resultParallel;

    double a[3] = {0, 0, 0};
    double b[3] = {1, 1, 1};
    double n[3] = {100, 100, 100};

    if (rank == 0) {
        resultSequential = integrateSequential(func, a, b, n);
    }

    resultParallel = integrateParallel(func, a, b, n);

    if (rank == 0) {
        // std::cout << "Parallel: " << resultParallel;
        // std::cout << "Sequential: " << resultSequential;
        ASSERT_NEAR(resultSequential, resultParallel, epsilon);
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
