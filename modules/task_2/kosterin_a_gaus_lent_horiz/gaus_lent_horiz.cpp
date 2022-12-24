// Copyright 2022 Kosterin Alexey
#include <mpi.h>
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
  double temp=0;
  int index=0;
  for (int i = 0; i < size; i++) {
    MPI_Bcast((a[i]), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast((b), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  double *x = new double[size];
  for (int k = 0; k < size; k++) {
    if(rank==0){
      if(a[k][k]==0){
        for(int i=k;i<size;i++){
          if(a[k][i]!=0){
            index=i;
            break;
          }
        }
        for(int i=0;i<size;i++){
          temp=a[i][k];
          a[i][k]=a[i][index];
          a[i][index]=temp;
        }
        for (int i = 0; i < size; i++) {
          MPI_Bcast((a[i]), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
          MPI_Bcast((b), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
      }
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
          MPI_Recv(a[delta * s + l], size, MPI_DOUBLE, s, 0, MPI_COMM_WORLD,
                   &status);
          MPI_Recv(&b[delta * s + l], size, MPI_DOUBLE, s, 0, MPI_COMM_WORLD,
                   &status);
        }
      }
    } else {
      for (int s = beg; s < iend; s++) {
        MPI_Send(a[s], size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&b[s], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      }
    }
  }
  for (int k = size - 1; k >= 0; k--) {
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
