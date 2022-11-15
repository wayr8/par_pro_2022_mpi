// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <vector>
#include <string>
#include <numeric>

int* getRandomMatrix(int x, int y); //x סעמכבצמג y סענמך

void getSequentialOperation(int* matrix,int* result, int x, int y);

void getParallelOperation(int* matrix,int*result, int size_x, int size_y);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
