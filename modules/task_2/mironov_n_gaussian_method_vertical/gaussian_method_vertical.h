  // Copyright 2022 Mironov Nikita
#ifndef MODULES_TASK_2_MIRONOV_N_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_VERTICAL_H_
#define MODULES_TASK_2_MIRONOV_N_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_VERTICAL_H_

#include <mpi.h>
#include <cassert>
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdexcept>

bool checkEqualOfMatrix(const std::vector <double> &firstMatrix,
                        const std::vector <double> &secondMatrix);
std::vector <double> getRandomMatrix(int rows, int cols,
                    double min_value, double max_value);
std::vector <double> nonParSolution(const std::vector <double> &coefs,
                                  size_t rows, size_t cols);
std::vector <double> ParSolution(const std::vector <double> &coefs,
                                  size_t rows, size_t cols);
bool checkSolution(const std::vector <double> &coefs, size_t rows, size_t cols,
                    const std::vector <double> &x);

#endif  // MODULES_TASK_2_MIRONOV_N_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_VERTICAL_H_
