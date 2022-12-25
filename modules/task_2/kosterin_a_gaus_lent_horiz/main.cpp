// Copyright 2022 Kosterin Alexey
#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include "./gaus_lent_horiz.h"
#include <gtest-mpi-listener.hpp>

std::random_device dev;
std::mt19937 gen(dev());
std::uniform_real_distribution<> dist(0, 1);
double **arr(int size) {
  double **ar = 0;
  ar = new double *[size];
  for (int i = 0; i < size; i++) {
    ar[i] = new double[size];
    for (int j = 0; j < size; j++) {
      double ran = static_cast<double>((gen() % 100));
      ar[i][j] = ran + i + j;
    }
  }

  return ar;
}

double *answer(int size) {
  double *ar = 0;
  ar = new double[size];
  for (int j = 0; j < size; j++) {
    double ran = static_cast<double>((gen() % 100));
    ar[j] = ran + 1;
  }
  return ar;
}

TEST(Parallel_Operations_MPI, Test_3x3) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 3;
  double **array = arr(size);
  double *ans = answer(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = Gaus(array, ans, size);
  int answer = ans[0];
  for (int i = 0; i < size; i++) {
    delete[] array[i];
  }
  delete[] array;
  delete[] ans;
  if (rank == 0) {
    EXPECT_NEAR(res, answer, err);
  }
}

TEST(Parallel_Operations_MPI, Test_5x5) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 5;
  double **array = arr(size);
  double *ans = answer(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = Gaus(array, ans, size);
  int answer = ans[0];
  for (int i = 0; i < size; i++) {
    delete[] array[i];
  }
  delete[] array;
  delete[] ans;
  if (rank == 0) {
    EXPECT_NEAR(res, answer, err);
  }
}
TEST(Parallel_Operations_MPI, Test_10x10) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 10;
  double **array = arr(size);
  double *ans = answer(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = Gaus(array, ans, size);
  int answer = ans[0];
  for (int i = 0; i < size; i++) {
    delete[] array[i];
  }
  delete[] array;
  delete[] ans;
  if (rank == 0) {
    EXPECT_NEAR(res, answer, err);
  }
}

TEST(Parallel_Operations_MPI, Test_15x15) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 15;
  double **array = arr(size);
  double *ans = answer(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = Gaus(array, ans, size);
  int answer = ans[0];
  for (int i = 0; i < size; i++) {
    delete[] array[i];
  }
  delete[] array;
  delete[] ans;
  if (rank == 0) {
    EXPECT_NEAR(res, answer, err);
  }
}

TEST(Parallel_Operations_MPI, Test_30x30) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 30;
  double **array = arr(size);
  double *ans = answer(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = Gaus(array, ans, size);
  int answer = ans[0];
  for (int i = 0; i < size; i++) {
    delete[] array[i];
  }
  delete[] array;
  delete[] ans;
  if (rank == 0) {
    EXPECT_NEAR(res, answer, err);
  }
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
  MPI_Finalize();
}
