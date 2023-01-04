// Copyright 2022 Yunin Daniil
#ifndef MODULES_TASKS_2_YUNIN_D_GAUSS_RIBBON_HORIZONTAL_SCHEME_GAUSS_RIBBON_HORIZONTAL_SCHEME_H_
#define MODULES_TASKS_2_YUNIN_D_GAUSS_RIBBON_HORIZONTAL_SCHEME_GAUSS_RIBBON_HORIZONTAL_SCHEME_H_

#include <vector>
#include <random>

using std::vector;
using std::mt19937;

vector<double> GaussConsequent(int matrix_size);
void UpdateRandNumbers(mt19937 *gen);
void CreateMatrix(vector<double> &matrix, int size);
void CreateVector(vector<double> &vec, int size);
void CreateMatrixRandom(vector<double> &matrix, int size, mt19937 *gen);
void CreateVectorRandom(vector<double> &vec, int size, mt19937 *gen);
vector<double> GaussParallel(vector<double> &matr, vector<double> &right_part, int size_matr);

#endif  // MODULES_TASKS_2_YUNIN_D_GAUSS_RIBBON_HORIZONTAL_SCHEME_GAUSS_RIBBON_HORIZONTAL_SCHEME_H_
