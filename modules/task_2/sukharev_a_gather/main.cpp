// Copyright 2022 Sukharev Artem

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <vector>
#include <iostrem>

#include <gtest-mpi-listener.hpp>
#include "./gather.h"

TEST(Gather_MPI, Test_IntVector) {
  int rank, ProcNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  std::vector<int> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % ProcNum;
    globalSize = gen() % 100 + 1;
    globalSize = globalSize * ProcNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / ProcNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), globalSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);
  
  double timeImb,timeOwn;
  std::vector<int> recImbVector(globalSize);
  std::vector<int> recOwnVector(globalSize);
  if(rank==root){
    timeImb = MPI_Wtime();
  }
  //

  if(rank==root){
    timeImb=MPI_Wtime()-timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(rank==root){
    timeOwn=MPI_Wtime();
  }
  //

  if(rank==root){
    timeOwn=MPI_Wtime()-timeOwn;
  }
  if(rank==root){
    ASSERT_EQ(recImbVector,recOwnVector);
    std::cout<<"Test1:\n";
    std::cout<<"timeImb = "<<timeImb<<", ";
    std::cout<<"timeOwn = "<<timeOwn<<".\n";
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}