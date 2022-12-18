#pragma once
// Copyright 2022 Korobeynikova Alisa
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <vector>
#include <string>

template <typename T>
class Matrix {
 private:
  std::vector<T> data;
  int n, m;

 public:
  Matrix(int _n, int _m) : data(std::vector<T>(_n * _m)), n(_n), m(_m) {}
  int GetN() const { return n; }
  int GetM() const { return m; }
  std::vector<T> GetData() { return data; }
  T* operator[](int pos) { return data.data() + pos * m; }
};

std::string vtos(std::vector<int>);
Matrix<int> getRandomMatrix(int n, int m);
std::vector<int> getParallelOperation(Matrix<int> global_matr);
int findMax(std::vector<int> vec);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
