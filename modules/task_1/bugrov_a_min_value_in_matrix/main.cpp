#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./min_value_in_matrix.h"

TEST(min_value_in_matrix, small_matr_no_reminder_2_proc) {
  int rank;
  int min_value_seq, min_value_par;
  const int kSize = 500;
  int matrix[kSize];
  const int kProcNum = 2;
  int min_values[kProcNum];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    CreateRandomValues(matrix, kSize);
  }
  ParMinValue(matrix, kSize, kProcNum, min_values);
  min_value_par = min_values[0];
  if (rank == 0) {
    min_value_seq = SeqMinValue(matrix, kSize);
    EXPECT_EQ(min_value_par, min_value_seq);
  }
}

TEST(min_value_in_matrix, big_matr_coprime_integers_4_proc) {
  int rank;
  int min_value_seq, min_value_par;
  const int kSize = 998001;
  int matrix[kSize];
  const int kProcNum = 2;
  int min_values[kProcNum];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    CreateRandomValues(matrix, kSize);
  }
  ParMinValue(matrix, kSize, kProcNum, min_values);
  min_value_par = min_values[0];
  if (rank == 0) {
    min_value_seq = SeqMinValue(matrix, kSize);
    EXPECT_EQ(min_value_par, min_value_seq);
  }
}

TEST(min_value_in_matrix, big_matr_coprime_integers_3_proc) {
  int rank;
  int min_value_seq, min_value_par;
  const int kSize = 1000000;
  int matrix[kSize];
  const int kProcNum = 3;
  int min_values[kProcNum];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    CreateRandomValues(matrix, kSize);
  }
  ParMinValue(matrix, kSize, kProcNum, min_values);
  min_value_par = min_values[0];
  if (rank == 0) {
    min_value_seq = SeqMinValue(matrix, kSize);
    EXPECT_EQ(min_value_par, min_value_seq);
  }
}
TEST(min_value_in_matrix, big_matr_coprime_integers_10_proc) {
  int rank;
  int min_value_seq, min_value_par;
  const int kSize = 1000998;
  int matrix[kSize];
  const int kProcNum = 10;
  int min_values[kProcNum];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  CreateRandomValues(matrix, kSize);
  ParMinValue(matrix, kSize, kProcNum, min_values);
  min_value_par = min_values[0];
  if (rank == 0) {
    min_value_seq = SeqMinValue(matrix, kSize);
    EXPECT_EQ(min_value_par, min_value_seq);
  }
}
TEST(min_value_in_matrix, big_matr_1_proc) {
  int rank;
  int min_value_seq, min_value_par;
  const int kSize = 1000998;
  int matrix[kSize];
  const int kProcNum = 1;
  int min_values[kProcNum];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  CreateRandomValues(matrix, kSize);
  ParMinValue(matrix, kSize, kProcNum, min_values);
  min_value_par = min_values[0];
  if (rank == 0) {
    min_value_seq = SeqMinValue(matrix, kSize);
    EXPECT_EQ(min_value_par, min_value_seq);
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
