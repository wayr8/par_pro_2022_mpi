#ifndef MODULES_TASK_1_NESTEROV_A_VECTOR_SUM_OPS_MPI_H_
#define MODULES_TASK_1_NESTEROV_A_VECTOR_SUM_OPS_MPI_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int row, int col);
int getSequentialOperations(std::vector<int> vector);
std::vector<int> getParallelOperations(std::vector<int> matrix, int count_row, int count_col);

#endif  // MODULES_TASK_1_NESTEROV_A_VECTOR_SUM_OPS_MPI_H_
