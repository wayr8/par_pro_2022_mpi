// Copyright 2022 Bugrov Andrey

#include "min_value_in_matrix.h"

#include <mpi.h>  // for mpi

#include <ctime>  // for time function
#include <random>  // for mt19937

void CreateRandomValues(int* matrix, int size) {
  std::mt19937 engine(time(0));
  for (int i = 0; i < size; i++) {
    matrix[i] = engine() % 100000;
  }
}

void MatrixPrinter(int* matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << matrix[i * m + j] << ' ';
    }
    std::cout << "\n";
  }
}

int SeqMinValue(int* matrix, int size) {
  int min_value = matrix[0];
  int i = 1;
  for (; i < size; i++) {
    if (matrix[i] < min_value) {
      min_value = matrix[i];
    }
  }
  return min_value;
}

// result is in min_values[0]
void ParMinValue(int* matrix, int size, int process_num, int* min_values) {
  int rank = 0;
  int part = size / process_num;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int i = 0;
    for (; i < process_num - 1; i++) {
      MPI_Send(matrix + i * part, part, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
    }
    min_values[0] = matrix[(process_num - 1) * part];
    for (i = (process_num - 1) * part + 1; i < size; i++) {
      if (min_values[0] > matrix[i]) {
        min_values[0] = matrix[i];
      }
    }
    for (int i = 1; i < process_num; i++) {
      MPI_Recv(min_values + i, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      if (min_values[0] > min_values[i]) {
        min_values[0] = min_values[i];
      }
    }
  } else {
    if (rank < process_num) {
      int* recv_buf;
      recv_buf = new int[part];
      int* tmp_result = new int;
      MPI_Recv(recv_buf, part, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      *tmp_result = recv_buf[0];
      int i = 1;
      min_values[rank] = recv_buf[0];
      for (; i < part; i++) {
        /*if (min_values[rank] > recv_buf[i]) {
            min_values[rank] = recv_buf[i];
        }*/
        if (*tmp_result > recv_buf[i]) {
          *tmp_result = recv_buf[i];
        }
      }
      MPI_Send(tmp_result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
}

// int ParMinValue(int* matrix, int size, int process_num)
//{
//    int rank = 0;
//    int part = size / process_num;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    int* min_values = new int[process_num];
//    if (rank == 0) {
//        MPI_Scatter(matrix, part, MPI_INT, 0, MPI_INT, MPI_COMM_WORLD);
//        int remainder = size - part * process_num;
//        if (remainder) {
//            int* zero_part = matrix + part * process_num;
//            min_values[0] = zero_part[0];
//            for (int i = 1; i < remainder; i++) {
//                if (min_values[0] > zero_part[i]) {
//                    min_values[0] = zero_part[i];
//                }
//            }
//        }
//    }
//    else {
//        int* recv_buf;
//        recv_buf = new int[part];
//        MPI_Scatter(matrix, part, MPI_INT, 0, MPI_INT, MPI_COMM_WORLD);
//        min_values[rank] = recv_buf[0];
//        for (int i = 1; i < part; i++) {
//            if (min_values[0] > recv_buf[i]) {
//                min_values[0] = recv_buf[i];
//            }
//        }
//    }
//}