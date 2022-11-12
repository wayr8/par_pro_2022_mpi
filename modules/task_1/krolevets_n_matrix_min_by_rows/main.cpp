// Copyright 2022 me
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./matrix_min_by_rows.h"

TEST(min_by_rows_test, can_generate_square_matrix) {
  int32_t size_x = 10000;
  int32_t size_y = 10000;
  uint64_t total = static_cast<uint64_t>(size_x) * size_y;
  uint32_t* matrix = new uint32_t[total];
  for (uint64_t i = 0; i < total; ++i) {
    matrix[i] = std::numeric_limits<uint32_t>::max();
  }
  generate_matrix(reinterpret_cast<int32_t*>(matrix), size_x, size_y);
  for (uint64_t i = 0; i < total; ++i) {
    ASSERT_NE(matrix[i], std::numeric_limits<uint32_t>::min());
  }
  delete[] matrix;
}

TEST(min_by_rows_test, can_generate_rectangle_matrix) {
  int32_t size_x = 10000;
  int32_t size_y = 1000;
  uint64_t total = static_cast<uint64_t>(size_x) * size_y;
  uint32_t* matrix = new uint32_t[total];
  for (uint64_t i = 0; i < total; ++i) {
    matrix[i] = std::numeric_limits<uint32_t>::max();
  }
  generate_matrix(reinterpret_cast<int32_t*>(matrix), size_x, size_y);
  for (uint64_t i = 0; i < total; ++i) {
    ASSERT_NE(matrix[i], std::numeric_limits<uint32_t>::min());
  }
  delete[] matrix;
}

TEST(min_by_rows_test, find_minimums_in_square_matrix) {
  int32_t size_x = 10000;
  int32_t size_y = 10000;
  uint64_t total = static_cast<uint64_t>(size_x) * size_y;
  int32_t* matrix = nullptr;
  int32_t* mpi_result = nullptr;
  int32_t* seq_result = nullptr;
  int32_t rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matrix = new int32_t[total];
    mpi_result = new int32_t[size_y];
    seq_result = new int32_t[size_y];
    generate_matrix(matrix, size_x, size_y);
    min_by_rows_seq(matrix, seq_result, size_x, size_y);
  }
  min_by_rows(matrix, mpi_result, size_x, size_y);
  if (rank == 0) {
    for (int i = 0; i < size_y; ++i) {
      EXPECT_EQ(mpi_result[i], seq_result[i]);
    }
    delete[] matrix;
    delete[] mpi_result;
    delete[] seq_result;
  }
}

TEST(min_by_rows_test, find_minimums_in_rectangle_matrix) {
  int32_t size_x = 1000;
  int32_t size_y = 10000;
  uint64_t total = static_cast<uint64_t>(size_x) * size_y;
  int32_t* matrix = nullptr;
  int32_t* mpi_result = nullptr;
  int32_t* seq_result = nullptr;
  int32_t rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matrix = new int32_t[total];
    mpi_result = new int32_t[size_y];
    seq_result = new int32_t[size_y];
    generate_matrix(matrix, size_x, size_y);
    min_by_rows_seq(matrix, seq_result, size_x, size_y);
  }
  min_by_rows(matrix, mpi_result, size_x, size_y);
  if (rank == 0) {
    for (int i = 0; i < size_y; ++i) {
      EXPECT_EQ(mpi_result[i], seq_result[i]);
    }
    delete[] matrix;
    delete[] mpi_result;
    delete[] seq_result;
  }
}

TEST(min_by_rows_test, size_y_less_then_num_of_processes) {
  int32_t size_x = 10000;
  int32_t size_y = 1;
  uint64_t total = static_cast<uint64_t>(size_x) * size_y;
  int32_t* matrix = nullptr;
  int32_t* mpi_result = nullptr;
  int32_t* seq_result = nullptr;
  int32_t rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matrix = new int32_t[total];
    mpi_result = new int32_t[size_y];
    seq_result = new int32_t[size_y];
    generate_matrix(matrix, size_x, size_y);
    min_by_rows_seq(matrix, seq_result, size_x, size_y);
  }
  min_by_rows(matrix, mpi_result, size_x, size_y);
  if (rank == 0) {
    for (int i = 0; i < size_y; ++i) {
      EXPECT_EQ(mpi_result[i], seq_result[i]);
    }
    delete[] matrix;
    delete[] mpi_result;
    delete[] seq_result;
  }
}

int main(int argc, char** argv) {
  // Filter out Google Test arguments
  ::testing::InitGoogleTest(&argc, argv);

  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Add object that will finalize MPI on exit; Google Test owns this pointer
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);

  // Get the event listener list.
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  // Remove default listener
  delete listeners.Release(listeners.default_result_printer());

  // Adds MPI listener; Google Test owns this pointer
  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

  // Run tests, then clean up and exit. RUN_ALL_TESTS() returns 0 if all tests
  // pass and 1 if some test fails.
  return RUN_ALL_TESTS();
}
