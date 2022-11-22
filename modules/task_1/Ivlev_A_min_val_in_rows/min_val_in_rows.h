  // Copyright 2022 Ivlev A
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <vector>
#include <string>

int* getRandomMatrix(int m, int n);
int min_in_vec(int* vec, int size);
int* getMatrixMinbyRow(int* global_matrix, int row_num, int column_num);
int* getParallelMin(int* global_matrix, int row_num, int column_num);


#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
