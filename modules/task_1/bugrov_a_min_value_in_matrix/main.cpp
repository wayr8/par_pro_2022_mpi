#include <gtest/gtest.h>
#include "./min_value_in_matrix.h"
#include <gtest-mpi-listener.hpp>
TEST(min_value_in_matrix, equals_value_no_reminder_2_proc)
{
    int rank;
    int min_value1, min_value2;
    const int size = 500;
    int matrix[size];
    int min_values[2];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        create_random_values(matrix, size);
        min_value1 = seq_min_value(matrix, size);
    }
    par_min_value(matrix, size, 2, min_values);
}
TEST(min_value_in_matrix, random_values_no_reminder_2_proc)
{

}
TEST(min_value_in_matrix, random_values_coprime_integers_3_proc)
{

}
TEST(min_value_in_matrix, random_values_coprime_integers_10_proc)
{

}
TEST(min_value_in_matrix, random_values_1_proc)
{

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