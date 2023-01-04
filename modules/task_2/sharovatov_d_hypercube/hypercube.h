// Copyright 2022 Sharovatov Daniil
#ifndef MODULES_TASK_2_SHAROVATOV_D_HYPERCUBE_H_
#define MODULES_TASK_2_SHAROVATOV_D_HYPERCUBE_H_

#include <vector>
#include <random>

#include "mpi.h"

int* getRandomVector(int size, int seed);

bool isHyperCube(int procNum);

void sendHyperCube(void* buf, int count, MPI_Datatype datatype, int root,
                   int dest, int tag, MPI_Comm comm, std::vector<int>* pathToDest);

#endif  // MODULES_TASK_2_SHAROVATOV_D_HYPERCUBE_H_