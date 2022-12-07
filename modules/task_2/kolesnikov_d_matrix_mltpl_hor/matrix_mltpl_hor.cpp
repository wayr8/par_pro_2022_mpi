// Copyright 2022 Kolesnikov Denis
#include "../../../modules/task_2/kolesnikov_d_matrix_mltpl_hor/matrix_mltpl_hor.h"

vector<int> GenRndMtrx(int size_x, int size_y) {
  std::random_device dev;
  std::mt19937_64 gen(dev());
  vector<int> result(size_x * size_y);
  for (int i = 0; i < size_x * size_y; ++i) {
    result[i] = gen() % 100;
  }
  return result;
}

int CoorldLin(int x, int y, int size_x) {
  return y * size_x + x;
}

vector<int> MatrixMtlplSeq(
    const vector<int>& a,
    int a_height,
    int a_width,
    const vector<int>& b,
    int b_height,
    int b_width) {
  if (a_height == 0 || a_width == 0 || b_height == 0 || b_width == 0) {
    return vector<int>();
  }
  int tmp;
  vector<int> c(a_height*b_width);
  for (int i = 0; i < a_height; i++) {
    for (int j = 0; j < b_width; j++) {
      c[i * b_width + j] = 0;
      tmp = 0;
      for (int k = 0; k < a_width; k ++) {
        tmp += a[i * a_width + k] * b[k * b_width + j];
      }
      c[i * b_width + j] = tmp;
    }
  }
  return c;
}

vector<int> MatrixMtlplPrl(
    const vector<int>& a,
    int a_height,
    int a_width,
    const vector<int>& b,
    int b_height,
    int b_width) {
  int proc_num;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<int> c(a_height*b_width);
  int n_process;
  int index;
  int tmp;
  MPI_Status Status;
  int proc_part_a = a_height / proc_num;
  int part_a = proc_part_a * a_width;
  int proc_part_b = b_width / proc_num;
  int part_b = proc_part_b * a_width;
  vector<int> buf_a(part_a);
  vector<int> buf_b(part_b);
  vector<int> buf_c(proc_part_a * b_width);
  MPI_Scatter(a.data(), part_a, MPI_INT, buf_a.data(), part_a, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(b.data(), part_b, MPI_INT, buf_b.data(), part_b, MPI_INT, 0, MPI_COMM_WORLD);
  tmp = 0;
  for (int i = 0; i < proc_part_a; i++) {
    for (int j = 0; j < proc_part_b; j++) {
      tmp = 0;
      for (int k = 0; k < a_width; k++) {
        tmp += buf_a[i * a_width + k] * buf_b[j * a_width + k];
      }
      buf_c[i * b_width + j + proc_part_a * rank] = tmp;
    }
  }
  int next_proc;
  int prev_proc;
  next_proc = rank + 1;
  if (rank == proc_num - 1) next_proc = 0;
  prev_proc = rank - 1;
  if (rank == 0) prev_proc = proc_num - 1;
  for (n_process = 1; n_process < proc_num; n_process++) {
    MPI_Sendrecv_replace(
      buf_b.data(),
      part_b,
      MPI_INT,
      next_proc,
      0,
      prev_proc,
      0,
      MPI_COMM_WORLD,
      &Status);
    tmp = 0;
    for (int i = 0; i < proc_part_a; i++) {
      for (int j = 0; j < proc_part_b; j++) {
        tmp = 0;
        for (int k = 0; k < a_width; k++)
          tmp += buf_a[i * a_width + k] * buf_b[j * a_width + k];
        if (rank - n_process >= 0)
          index = rank - n_process;
        else
          index =(proc_num - n_process + rank);
        buf_c[i * b_width + j + index * proc_part_a] = tmp;
      }
    }
  }
  MPI_Gather(
    buf_c.data(),
    proc_part_a * b_width,
    MPI_INT,
    c.data(),
    proc_part_a * b_width,
    MPI_INT,
    0,
    MPI_COMM_WORLD);
  return c;
}
