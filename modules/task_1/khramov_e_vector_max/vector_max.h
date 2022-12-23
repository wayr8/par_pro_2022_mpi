// Copyright 2022 Khramov Egor
#ifndef MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_
#define MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_

#include <vector>
#include <string>

void printVector(std::vector<int> vec);

std::vector<int> getRandomVector(int size);

int getMax(std::vector<double> vec);

int getMaxParallel(std::vector<double> globalVector, int global_vector_size);

#endif  // MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_
