// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#include "./strongin_method.h"

namespace {
double function1(double x) {
  return sqrt(1 + 3 * cos(x) * cos(x)) + cos(10 * x);
}
}  // namespace

TEST(Parallel_Operations_MPI, Test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
  auto min = GetMinParallel(function1, 1, 50, 0.01);
  
  if (rank == 0) {
    ASSERT_NEAR(min, 0, 0.01);
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
