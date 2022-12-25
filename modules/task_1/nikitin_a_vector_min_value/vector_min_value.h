// Copyright 2022 Nikitin Alexander
#ifndef MODULES_TASK_1_NIKITIN_A_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_
#define MODULES_TASK_1_NIKITIN_A_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_

#include <string>
#include <vector>

std::vector<int> RandomVector(int vecSize);

double VectorMin(const std::vector<int>& vec);

double VectorMinParralel(const std::vector<int>& globVec, const int vecSize);

#endif  // MODULES_TASK_1_NIKITIN_A_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_
