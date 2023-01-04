// Copyright 2022 Sharovatov Daniil
#ifndef MODULES_TASK_1_SHAROVATOV_D_SCALAR_PRODUCT_H_
#define MODULES_TASK_1_SHAROVATOV_D_SCALAR_PRODUCT_H_

#include <vector>
#include <random>

#include "mpi.h";


std::vector<int> getRandomVector(int size);

int getScalarProductSequential(std::vector<int> v1, std::vector<int> v2);

int getScalarProductParallel(std::vector<int> v1, std::vector<int> v2);

#endif  // MODULES_TASK_1_SHAROVATOV_D_SCALAR_PRODUCT_H_
