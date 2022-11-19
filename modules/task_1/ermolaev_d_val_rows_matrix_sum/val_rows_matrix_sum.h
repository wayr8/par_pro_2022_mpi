// Copyright 2022 Ermolaev Danila
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>

int* getRandomMatrix(int x, int y);

void getSequentialOperation(int* matrix, int* result, int x, int y);

void getParallelOperation(int* matrix, int* result, int size_x, int size_y);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
