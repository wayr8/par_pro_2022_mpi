// Copyright 2022 Bugrov Andrey

#ifndef MODULES_TASK_1_BUGROV_A_MIN_VALUE_IN_MATRIX_H_
#define MODULES_TASK_1_BUGROV_A_MIN_VALUE_IN_MATRIX_H_

void CreateRandomValues(int* matrix, int size);
void MatrixPrinter(int* matrix, int n, int m);
int SeqMinValue(int* matrix, int size);
void ParMinValue(int* matrix, int size, int process_num, int* min_values);

#endif  // MODULES_TASK_1_BUGROV_A_MIN_VALUE_IN_MATRIX_H_