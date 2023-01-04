// Copyright 2023 Raschetnov Alexei
#include <mpi.h>
#include <random>
#include <iostream>
#include "../../../modules/task_2/raschetnov_a_ring/ring.h"

int getResult(int information, int size, int delta) {
  for (int i = 0; i < size - 1; i++)
    information += delta;
  return information;
}

int SendRingParallel(int information, int count, MPI_Datatype datatype,
                     int root, int delta, int tag, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size == 1)
    return information;
  if (rank == root) {
    MPI_Status status;
    int dest = rank - 1;
    if (dest < 0)
      dest = size - 1;
    MPI_Send(&information, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    MPI_Recv(&information, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &status);

    return information;
  } else {
    MPI_Status status;
    int dest = rank - 1;
    if (dest < 0)
      dest = size - 1;
    MPI_Recv(&information, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &status);
    information += delta;

    MPI_Send(&information, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  }
}

int getRandomNumber(int leftborder, int rightborder) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(leftborder, rightborder);

  return distr(gen);
}
