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

// int constructPath(int* result, int root, int destination, int size) {
//   return 0;
// }

std::vector<int> constructPath(int root, int destination, int size) {
  std::vector<int> result;
  int i = root;
  //  std::cout << size << std::endl;
  while (i != destination) {
    //  std::cout << i << std::endl;
    result.push_back(i);
    i = (i + 1) % size;
  }
  result.push_back(i);
  return result;
}

int SendRingParallel(int information, int count, MPI_Datatype datatype,
                     int root, int destination, int tag, MPI_Comm comm) {
  if (root == destination)
    return information;
  int world_size, world_rank;
  MPI_Comm new_comm;
  MPI_Group old_group, new_group;
  //  const int ranks[4] = {3, 0, 1, 2};
  //  std::vector<int> ranks = {3, 0, 1, 2};
  //  int max = getMax(ranks, n);
  //  std::cout << max << std::endl;

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  std::vector<int> ranks = constructPath(root, destination, world_size);
  // if (world_rank == 0)
  //   for (int i = 0; i < ranks.size(); i++)
  //     std::cout << ranks[i] << std::endl;

  MPI_Comm_group(MPI_COMM_WORLD, &old_group);
  MPI_Group_incl(old_group, ranks.size(), ranks.data(), &new_group);

  MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

  int prime_rank, prime_size;

  if (MPI_COMM_NULL != new_comm) {
    MPI_Status status;
    MPI_Comm_rank(new_comm, &prime_rank);
    MPI_Comm_size(new_comm, &prime_size);
    //  std::cout << "Prime rank: " << prime_rank << world_rank << std::endl;
    //  std::cout << "World rank: " << world_rank << std::endl;
    //  printf("Prime rank: %d, World rank %d", prime_rank, world_rank);
    if (prime_rank == 0) {
      MPI_Send(&information, 1, MPI_INT, (prime_rank + 1) % prime_size, 0, new_comm);
      //  MPI_Send(&information, 1, MPI_INT, (prime_rank + 1) % (max + 1), 0, new_comm);
      int source = prime_rank - 1;
      MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
      return information;
      //  std::cout << "Result from proc: " << prime_rank << ", information received: " << information << std::endl;
    } else {
    int source = prime_rank - 1;
    if (prime_rank == 0)
      source = prime_size - 1;
    MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
    //  std::cout << "Received from proc: " << source <<
    // " to proc " << prime_rank << " value: " << information << std::endl;
    information += 1;
    MPI_Send(&information, 1, MPI_INT, (prime_rank + 1) % prime_size, 0, new_comm);
    }
    //  std::cout << prime_rank << std::endl;
  }

  return information;

  // if (rank == root) {
  //   int dest = (rank + 1) % n;
  //   int source = rank - 1;
  //   MPI_Status status;
  //   MPI_Send(&information, 1, MPI_INT, dest, 0, new_comm);

  //   if (rank == 0)
  //     source = n - 1;
  //   MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
  //   std::cout << "Result: " << information << std::endl;
  //   return information;
  // }
  // else {
  //   int dest = (rank + 1) % n;
  //   int source = n - 1;
  //   MPI_Status status;

  //   if (rank == 0)
  //     source = n - 1;
  //   MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
  //   std::cout << "Receveid from proc " << source << " to proc " << rank << std::endl;
  //   information += 3;
  //   MPI_Send(&information, 1, MPI_INT, dest, 0, new_comm);
  // }
}

int getRandomNumber(int leftborder, int rightborder) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(leftborder, rightborder);

  return distr(gen);
}
