// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#include "./strongin_method.h"

TEST(Parallel_Operations_MPI, Test_1) {
  auto min = GetMin(std::cos, M_PI / 2, M_PI * 3 / 2, 0.01);
  ASSERT_NEAR(min, -1, 0.01);
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
