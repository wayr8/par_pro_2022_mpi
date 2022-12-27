// Copyright 2022 Pashina Alina

#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "../../../modules/task_2/pashina_a_bubble_sort/pashina_a_bubble_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(generatearr_MPI, can_create_arr) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  if (t_rank == 0) {
    int n = 3;
    ASSERT_NO_THROW(generateNewArr(n));
  }
}

TEST(doParallel_BubbleOddEven_MPI, can_do_para4) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  int n = 4;
  double *a = new double[n];
  a = generateNewArr(n);
  double *b = new double[n];
  ASSERT_NO_THROW(doParallel_BubbleOddEven(n, a, 0, MPI_COMM_WORLD));
}
TEST(generatearr_MPI, can_create_img) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  int n = 4;
  double *a = new double[n];
  a = generateNewArr(n);
  double *b = new double[n];
  if (t_rank == 0) ASSERT_NO_THROW(typical_bubble_s(a, n));
}

TEST(doParallel_BubbleOddEven_MPI, can_do_paral2) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  int n = 2;
  double *a = new double[n];
  a = generateNewArr(n);
  double *b = new double[n];
  ASSERT_NO_THROW(doParallel_BubbleOddEven(n, a, 0, MPI_COMM_WORLD));
}
TEST(doParallel_BubbleOddEven_MPI, can_do_paral5) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  int n = 5;
  double *a = new double[n];
  a = generateNewArr(n);
  double *b = new double[n];
  ASSERT_NO_THROW(doParallel_BubbleOddEven(n, a, 0, MPI_COMM_WORLD));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
