#ifndef ASTR_MATRIX_COLUMN_SUM_H
#define ASTR_MATRIX_COLUMN_SUM_H

#include <vector>

std::vector<int> generateRandomMatrix(int xSize, int ySize);
int linearizeCoordinates(int x, int y, int xSize);

std::vector<int> calculateMatrixSumParallel(std::vector<int>& matrix, int xSize, int ySize);
std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize);
std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize, int fromX, int toX);

#endif  // ASTR_MATRIX_COLUMN_SUM_H
