// Copyright 2022 Kosterin Alexey
#include <mpi.h>
#include <cmath>

#include "../../../modules/task_2/kosterin_a_gaus_lent_horiz/gaus_lent_horiz.h"

double Gaus(double **a, double *b, int size) {
  double d, s;
  int sizeProc, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
  int delta = size / sizeProc;
  int ibeg = rank * delta;
  int beg = ibeg;
  int iend = (rank + 1) * delta;
  int ost = size % sizeProc;
  double temp = 0, max = 0;
  int index = 0;

  double *x = new double[size];
  for (int i = 0; i < size; i++) {
    x[i] = 0;
  }
  for (int k = 0; k < size; k++) {
    if (rank == 0) {
      if (a[k][k] == 0) {
        for (int i = k; i < size; i++) {
          if (abs(a[i][k]) > max) {
            index = i;
            max = abs(a[i][k]);
          }
        }
        for (int i = 0; i < size; i++) {
          temp = a[k][i];
          a[k][i] = a[index][i];
          a[index][i] = temp;
        }
        temp = b[k];
        b[k] = b[index];
        b[index] = temp;
        if (max == 0) {
          k++;
        }
        max = 0;
      }
    }
    for (int i = 0; i < size; i++) {
      MPI_Bcast((a[i]), size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast((b), size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    }
    if ((rank == sizeProc - 1) && (ost != 0)) {
      iend = size;
    }
    if (ibeg == k)
      ibeg++;
    for (int j = ibeg; j < iend; j++) {

      d = a[j][k] / a[k][k];
      for (int i = k; i < size; i++) {
        a[j][i] = a[j][i] - d * a[k][i];
      }
      b[j] = b[j] - d * b[k];
    }
    if (rank == 0) {
      for (int s = 1; s < sizeProc; ++s) {
        MPI_Status status;
        int of = 0;
        if ((ost != 0) && (s == sizeProc - 1))
          of = ost;
        for (int l = 0; l < delta + of; l++) {
          MPI_Recv(a[delta * s + l], size, MPI_LONG_DOUBLE, s, 0,
                   MPI_COMM_WORLD, &status);
          MPI_Recv(&b[delta * s + l], size, MPI_LONG_DOUBLE, s, 0,
                   MPI_COMM_WORLD, &status);
        }
      }
    } else {
      for (int s = beg; s < iend; s++) {
        MPI_Send(a[s], size, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&b[s], 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
      }
    }
  }
  for (int k = size - 1; k >= 0; k--) {
    if (a[k][k] == 0) {
      a[k][k]++;
    }
    d = 0;
    for (int j = k + 1; j <= size; j++) {
      s = a[k][j] * x[j];
      d = d + s;
    }
    x[k] = (b[k] - d) / a[k][k];
  }
  double result = 0;
  for (int i = 0; i < size; i++) {
    result += a[0][i] * x[i];
  }
  return result;
}
