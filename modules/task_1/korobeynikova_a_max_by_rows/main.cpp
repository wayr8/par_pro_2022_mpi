// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <fstream>
#include <vector>

#include <gtest-mpi-listener.hpp>

#include "./max_by_rows.h"

TEST(Parallel_Operations_MPI, Test_Max) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 2, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    global_matr = getRandomMatrix(n_size_matrix, m_size_matrix);
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values[i] =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    ASSERT_EQ(max_values, seq_max_values);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<int>> global_matr;
  const int n_size_matrix = 5, m_size_matrix = 8;

  /*if (rank == 0) {
      global_matr = getRandomMatrix(n_size_matrix, m_size_matrix);
  }

  int global_max;
  global_max = getParallelOperations(global_matr, count_size_vector, "max");

  if (rank == 0) {
      int reference_max = getSequentialOperations(global_matr, "max");
      ASSERT_EQ(reference_max, global_max);
  }
  */
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
