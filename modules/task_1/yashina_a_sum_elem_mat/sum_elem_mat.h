// Copyright 2022 Yashina Anastasia
#ifndef MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_
#define MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

// основная работа
int SumFull(int size, std::vector<int> matrix);

// работа с отдельной частью, вычисление суммы
int SumPartly(std::vector<int> matrix);

std::vector<int> getMatrix(int size);

#endif  // MODULES_TASK_1_YASHINA_A_SUM_ELEM_MAT_SUM_ELEM_MAT_H_
