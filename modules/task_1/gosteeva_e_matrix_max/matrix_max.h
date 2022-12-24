
// Copyright 2022 Gosteeva Ekaterina
#ifndef MODULES_TASK_1_GOSTEEVA_E_MATRIX_MAX_MATRIX_MAX_H
#define MODULES_TASK_1_GOSTEEVA_E_MATRIX_MAX_MATRIX_MAX_H

#include <vector>
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>

std::vector<int> GetRandomMatrix(int matrix_size);
int GetMatrixMaxSequential(std::vector<int> &matrix);
int GetMatrixMaxParralel(std::vector<int> &matrix, const int size);

#endif // MODULES_TASK_1_GOSTEEVA_E_MATRIX_MAX_MATRIX_MAX_H