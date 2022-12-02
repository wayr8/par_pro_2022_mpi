// Copyright 2022 Semenova Veronika
#include <gtest/gtest.h>
#include <vector>
#include "./method_conjugate_gradient.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Serial_method_gradient_is_correct2x2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 2;
  Vector V(n);
  Vector M(n * n);
  Vector correct_res(n);
  V[0] = 17;
  V[1] = 16;
  M[0] = 1;
  M[1] = 2;
  M[2] = 2;
  M[3] = 1;
  correct_res[0] = 5;
  correct_res[1] = 6;
  if (rank == 0) {
    Vector res = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res.size(); i++)
      EXPECT_LE(abs(correct_res[i] - res[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Serial_method_gradient_is_correct3x3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 3;
  Vector V(n);
  Vector M(n * n);
  Vector correct_res(n);
  V[0] = 6;
  V[1] = 6;
  V[2] = 10;
  M[0] = 1;
  M[1] = 2;
  M[2] = 3;
  M[3] = 2;
  M[4] = 1;
  M[5] = 0;
  M[6] = 3;
  M[7] = 0;
  M[8] = 1;
  correct_res[0] = 3;
  correct_res[1] = 0;
  correct_res[2] = 1;
  if (rank == 0) {
    Vector res = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res.size(); i++)
      EXPECT_LE(abs(correct_res[i] - res[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_is_correct2x2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 2;
  Vector V(n);
  Vector M(n * n);
  Vector correct_res(n);
  V[0] = 17;
  V[1] = 16;
  M[0] = 1;
  M[1] = 2;
  M[2] = 2;
  M[3] = 1;
  correct_res[0] = 5;
  correct_res[1] = 6;
  Vector parl_res = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    for (int i = 0; i < parl_res.size(); i++)
      ASSERT_NEAR(correct_res[i], parl_res[i], 0.5);
  }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_is_correct3x3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 3;
  Vector V(n);
  Vector M(n * n);
  Vector correct_res(n);
  V[0] = 6;
  V[1] = 6;
  V[2] = 10;
  M[0] = 1;
  M[1] = 2;
  M[2] = 3;
  M[3] = 2;
  M[4] = 1;
  M[5] = 0;
  M[6] = 3;
  M[7] = 0;
  M[8] = 1;
  correct_res[0] = 3;
  correct_res[1] = 0;
  correct_res[2] = 1;
  Vector parl_res = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    for (int i = 0; i < parl_res.size(); i++)
      ASSERT_NEAR(correct_res[i], parl_res[i], 0.5);
  }
}
TEST(Parallel_Operations_MPI, Serial_and_paralle_method_with_random_6X6) {
  int rank;
  int n = 6;
  Vector V;
  Vector M;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  V = RandVec(n);
  M = RandMat(n);
  Vector parl_res = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    Vector res = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res.size(); i++) {
      ASSERT_NEAR(res[i], parl_res[i], 0.5);
    }
  }
}
TEST(Parallel_Operations_MPI, Serial_and_paralle_method_with_random10x10) {
  int rank;
  int n = 10;
  Vector V;
  Vector M;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  V = RandVec(n);
  M = RandMat(n);
  Vector parl_res = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    Vector res = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res.size(); i++) {
      ASSERT_NEAR(res[i], parl_res[i], 0.5);
    }
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
