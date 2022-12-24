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
      ar[i][j] = ran + 1;
    }
  }
  for(int i = 0; i < size; i++){
    ar[i][i] = ar[i][i] * 2;
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

TEST(Parallel_Operations_MPI, Test_4x4) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 4;
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

TEST(Parallel_Operations_MPI, Test_20x20) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 20;
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
TEST(Parallel_Operations_MPI, Test_50x50) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 50;
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

TEST(Parallel_Operations_MPI, Test_100x100) {
  bool flag = false;
  double err = 0.1;
  int rank;
  int size = 100;
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
