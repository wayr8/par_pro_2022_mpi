#ifndef MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_
#define MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_

#include <vector>
#include <mpi.h>
#include <random>

using std::vector;

int CoorldLin(int x, int y, int size_x);
vector<int> GenRndMtrx(int size_x, int size_y);
vector<int> MatrixMtlplSeq(vector<int>& a, int a_size_x, int a_size_y, vector<int>& b, int b_size_x, int b_size_y);
vector<int> MatrixMtlplPrl(vector<int>& a, int a_size_x, int a_size_y, vector<int>& b, int b_size_x, int b_size_y);

#endif // MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_