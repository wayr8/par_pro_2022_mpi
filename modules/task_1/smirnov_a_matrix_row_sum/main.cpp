// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_row_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    std::vector<int> result;
    const int count_rows_matrix = 12;
    const int count_columns_matrix = 4;

    if (rank == 0) {
        global_matrix = getRandomMatrix(count_rows_matrix, count_columns_matrix);
    }

    result = calculateParallelMatrixRowSum(global_matrix, count_rows_matrix, count_columns_matrix);

    if (rank == 0) {
        std::vector<int> reference_result = calculateSequentialMatrixRowSum(global_matrix,
          count_rows_matrix, count_columns_matrix);
        ASSERT_EQ(reference_result, result);
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
