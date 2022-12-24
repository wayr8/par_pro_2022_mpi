// Copyright 2022 Kandrin Alexey
#ifndef MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
#define MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_

#include <vector>

using Function = double (*)(double);

double GetMin(Function&& f, double a, double b, double epsilon);

#endif  // MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
