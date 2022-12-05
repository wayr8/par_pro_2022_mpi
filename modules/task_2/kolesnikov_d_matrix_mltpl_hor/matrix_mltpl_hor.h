// Copyright 2022 Kolesnikov Denis

#ifndef MODULES_TASK_2_KOLESNIKOV_D_MATRIX_MLTPL_HOR_MATRIX_MLTPL_HOR_H_
#define MODULES_TASK_2_KOLESNIKOV_D_MATRIX_MLTPL_HOR_MATRIX_MLTPL_HOR_H_

#include <mpi.h>
#include <vector>
#include <random>

using std::vector;

int CoorldLin(int x, int y, int size_x);
vector<int> GenRndMtrx(int size_x, int size_y);
vector<int> MatrixMtlplSeq(
    const vector<int>& a,
    int a_size_x,
    int a_size_y,
    const vector<int>& b,
    int b_size_x,
    int b_size_y);
vector<int> MatrixMtlplPrl(
    const vector<int>& a,
    int a_size_x,
    int a_size_y,
    const vector<int>& b,
    int b_size_x,
    int b_size_y);

#endif  // MODULES_TASK_2_KOLESNIKOV_D_MATRIX_MLTPL_HOR_MATRIX_MLTPL_HOR_H_
