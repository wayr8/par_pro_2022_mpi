// Copyright 2022 Ivlev A
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <vector>
#include <string>

int* getRandomMatrix(unsigned int m, unsigned int n);
int min_in_vec(int* vec, size_t size);
int* getMatrixMinbyRow(int* global_matrix, size_t row_num, size_t column_num);
int* getParallelMin(int* global_matrix, size_t row_num, size_t column_num);


#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
