// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <fstream>
#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./max_by_rows.h"

TEST(Parallel_Operations_MPI, Test_Max) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 9, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values.at(i) =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << "Max: ";
    outf << vtos(max_values) << std::endl;
    outf << "Seq_Max: ";
    outf << vtos(seq_max_values) << std::endl;
    ASSERT_EQ(max_values, seq_max_values);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 6, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values.at(i) =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    ASSERT_EQ(max_values, seq_max_values);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 3, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values.at(i) =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    ASSERT_EQ(max_values, seq_max_values);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 12, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values.at(i) =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    ASSERT_EQ(max_values, seq_max_values);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n_size_matrix = 1, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  if (rank == 0) {
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                       std::ios_base::app);
    outf << "Test\n";
    outf << vtos(global_matr.GetData()) << std::endl;
  }

  std::vector<int> max_values;
  max_values = getParallelOperation(global_matr);
  if (rank == 0) {
    std::vector<int> seq_max_values(n_size_matrix);
    for (int i = 0; i < n_size_matrix; ++i) {
      seq_max_values.at(i) =
          findMax(std::vector<int>(global_matr[i], global_matr[i + 1]));
    }
    ASSERT_EQ(max_values, seq_max_values);
  }
}

/*int main() {
  int tests[][2] = {{3, 5}, {5, 3}, {1, 5}, {5, 1}, {3, 3}, {3, 9}};
  int tests_num = 6;
  for (int test = 0; test < tests_num; ++test) {
    std::vector<int> task_distrib = taskDistrib(tests[test][0], tests[test][1]);
    for (int i = 0; i < task_distrib.size(); ++i) {
      std::cout << task_distrib.at(i) << ' ';
    }
    std::cout << std::endl;
  }
}
*/

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
