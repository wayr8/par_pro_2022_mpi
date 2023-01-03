// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
#define MODULES_TASK3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

#include <vector>
#include <string>
class SparseMatrix
{
public:
  std::vector<double> values;
  std::vector<int> columnIndex;
  std::vector<int> rowIndex;
  int countRows;
  int countColumns;
public:
  SparseMatrix(std::vector<std::vector<double>> matrix);
  //SparseMatrix(const SparseMatrix& m);
  int GetCountRows() { return countRows; }
  int GetCountColumns() { return countColumns; }
  std::vector<double> GetValues() { return values; }
  std::vector<int> GetColumns() { return columnIndex; }
  std::vector<int> GetPoints() { return rowIndex; }

  SparseMatrix Transpose();
  SparseMatrix() = default;
  void PrintStats();

  bool operator ==(SparseMatrix m) const;
};

SparseMatrix seqSparseMatrixMult(std::vector<std::vector<double>> _A, std::vector<std::vector<double>> _B);
SparseMatrix parSparseMatrixMult(std::vector<std::vector<double>> _A, std::vector<std::vector<double>> _B);




#endif  // MODULES_TASK3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
