// Copyright 2022 Nikitin Alexander
#include "../../../modules/task_1/nikitin_a_vector_min_value/vector_min_value.h"

#include <mpi.h>

#include <random>
#include <vector>

std::vector<int> RandomVector(int vecSize) {
  if (vecSize > 0) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<int> vectr(vecSize);
    for (int i = 0; i < vecSize; i++) vectr[i] = gen() % 100;
    return vectr;
  }
  throw "Error: Size is small!";
}

double VectorMin(const std::vector<int>& vectr) {
  int res = vectr[0];

  for (int i = 1; i < static_cast<int>(vectr.size()); i++) {
    if (vectr[i] < res) {
      res = vectr[i];
    }
  }

  return res;
}

double VectorMinParralel(const std::vector<int>& globVec, const int size) {
  int cSize, pRank;
  int min = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &cSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &pRank);

  int count = size / cSize;
  int rem = size % cSize;

  if (cSize == 0 && count > 0) {
    for (int pRank = 1; pRank <= cSize - 1; pRank++) {
      MPI_Send(globVec.data() + rem + pRank * count, count, MPI_INT, pRank, 0,
               MPI_COMM_WORLD);
    }
  }

  std::vector<int> local(count);
  if (pRank == 0) {
    local = std::vector<int>(globVec.begin(), globVec.begin() + (count + rem));
  } else if (count > 0) {
    MPI_Status status;
    MPI_Recv(local.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  if (count > 0) {
    int localMin = VectorMin(local);
    MPI_Reduce(&localMin, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  } else if (count == 0 && pRank == 0) {
    min = VectorMin(local);
  }

  return min;
}
