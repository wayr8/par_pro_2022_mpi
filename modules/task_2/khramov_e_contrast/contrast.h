// Copyright 2022 Khramov Egor
#ifndef  MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_
#define  MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_

#include <vector>

void printVector(std::vector<int> vector);

std::vector<int> getRandomVector(int size);

std::vector<int> getRandomMatrix(int w, int h);

float getContrastCoeff(int contrast);

int truncate(size_t value);

std::vector<int> getContrastedMatrixSequential(std::vector<int> matrix);

std::vector<int> getContrastedMatrixParallel(std::vector<int> matrix);

#endif  // MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_
