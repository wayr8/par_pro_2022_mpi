  // Copyright 2022 Mironov Nikita
#include <gtest/gtest.h>
#include "../../../modules/task_2/mironov_n_gaussian_method_vertical/gaussian_method_vertical.h"
#include <gtest-mpi-listener.hpp>

TEST(gaussian_method_vertical, matrix_2_rows_3_columns_test) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 2;
    const size_t columns = 3;

    if (procId == 0) {
        coefs = {
            -1, 1, 5,
            2, -1, -7
        };
    }

    std::vector <double> parSolution = ParSolution(coefs, rows, columns);
    if (procId == 0) {
        std::vector <double> nonParSolution =
            nonParSolution(coefs, rows, columns);
        ASSERT_TRUE(checkEqual(nonParSolution, parSolution));
        ASSERT_TRUE(checkSolution(coefs, rows, columns, parSolution));
    }
}

TEST(gaussian_method_vertical,  matrix_3_rows_4_columns_test) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -4, 9,
            3, 4, 1, 3,
            1, 0, 3, 5
        };
    }

    std::vector <double> parSolution = ParSolution(coefs, rows, columns);
    if (procId == 0) {
        std::vector <double> nonParSolution =
            nonParSolution(coefs, rows, columns);
        ASSERT_TRUE(checkEqual(nonParSolution, parSolution));
        ASSERT_TRUE(checkSolution(coefs, rows, columns, parSolution));
    }
}

TEST(gaussian_method_vertical,
    par_perfomance_check_with_incorrect_columns_num_1) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    EXPECT_ANY_THROW(ParSolution(coefs, rows, columns + 1));
}

TEST(gaussian_method_vertical,
    non_par_perfomance_check_with_incorrect_columns_num) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    if (procId == 0) {
        EXPECT_ANY_THROW(nonParSolution(coefs, rows, columns + 1));
    }
}

TEST(gaussian_method_vertical,
    par_perfomance_check_with_incorrect_columns_num_2) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1,
            1, -2, 1,
            1, 0, 2
        };
    }

    EXPECT_ANY_THROW(ParSolution(coefs, rows, columns));
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
