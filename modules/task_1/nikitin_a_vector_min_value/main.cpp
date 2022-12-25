// Copyright 2022 Nikitin Alexander
#include <gtest/gtest.h>

#include <vector>
#include "./vector_min_value.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, TEST_FIND_MIN_VAL_IN_VECTOR) {
  int commRank;
  std::vector<int> vector;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  if (commRank == 0) {
    vector = RandomVector(100);
  }

  int minFirst = VectorMin(vector);

  if (commRank == 0) {
    int minSecond = VectorMin(vector);
    ASSERT_EQ(minFirst, minSecond);
  }
}

TEST(Parallel_Operations_MPI, TEST_PARALLEL_FIND_MIN_VAL_IN_VECTOR_GET_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFirst, minSecond;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = RandomVector(100);
    minFirst = VectorMin(vector);
  }
  minSecond = VectorMinParralel(vector, 100);
  if (commRank == 0) {
    ASSERT_EQ(minFirst, minSecond);
  }
}

TEST(Parallel_Operations_MPI,
     TEST_PUT_NEGATIVE_SIZE_VALUE_TO_FUNCTION_FILL_VECTOR) {
  int commRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  ASSERT_ANY_THROW(RandomVector(-15));
}

TEST(Parallel_Operations_MPI,
     TEST_SIZE_VECTOR_LARGE_VALUE_TO_FUNCTION_PARALLEL_FIND_GET_MIN_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFirst, minSecond;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = RandomVector(20000000);
    minFirst = VectorMin(vector);
  }
  minSecond = VectorMinParralel(vector, 20000000);
  if (commRank == 0) {
    ASSERT_EQ(minFirst, minSecond);
  }
}

TEST(Parallel_Operations_MPI,
     TEST_PUT_VECTOR_HAS_FEWER_ELEMENTS_THAN_CPU_CORES_GET_MIN_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFirst, minSecond;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = RandomVector(4);
    minFirst = VectorMin(vector);
  }
  minSecond = VectorMinParralel(vector, 4);
  if (commRank == 0) {
    ASSERT_EQ(minFirst, minSecond);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &testEvent =
      ::testing::UnitTest::GetInstance()->listeners();

  testEvent.Release(testEvent.default_result_printer());
  testEvent.Release(testEvent.default_xml_generator());

  testEvent.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
