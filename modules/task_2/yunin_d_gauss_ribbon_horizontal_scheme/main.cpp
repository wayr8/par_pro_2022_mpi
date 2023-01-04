// Copyright 2022 Yunin Daniil
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_ribbon_horizontal_scheme.h"
#include <gtest-mpi-listener.hpp>

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 5;
    if (rank == 0) {
        global_vec = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            std::cout << global_vec[i] << ' ';
        }
        std::cout << std::endl;
        ASSERT_EQ(1, 1);
    }
        
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;

    const int count_size_vector = 5;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    CreateMatrix(matr, count_size_vector);
    CreateVector(right_vect, count_size_vector);
    global_vec = GaussParallel(matr, right_vect, count_size_vector);
    if (rank == 0) {
        ASSERT_EQ(1, 1);
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
