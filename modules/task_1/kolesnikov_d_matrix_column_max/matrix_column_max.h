#ifndef MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_
#define MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_

#include <vector>
#include <mpi.h>
#include <random>

using std::vector;

int coord_lin(int x, int y, int size_x);
vector<int> generate_rnd_matrix(int size_x, int size_y);
vector<int> max_by_column_prl(vector<int>& matrix, int size_x, int size_y);
vector<int> max_by_column_seq(const vector<int>& matrix, int size_x, int size_y);

#endif // MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MPI_H_