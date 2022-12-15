#pragma once
#include <vector>

using std::vector;

vector<int> CreateRandomVector(int v_size, int right_border, int offset);
void UpdateRandNumbers();
int CountErrorsOrderNeigboringElementsVector(const vector<int> &my_vector);
int CountErrorsOrderNeigboringElementsVectorParallel(const vector<int> &my_vector);