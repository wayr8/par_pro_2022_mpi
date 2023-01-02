// Copyright 2022 Sukharev Artem

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

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
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
  }
}
TEST(Gather_MPI, Test_IntVector1) {
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
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
  }
}
TEST(Gather_MPI, Test_IntVector2) {
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
    globalSize = gen() % 1000 + 1;
    globalSize = globalSize * ProcNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / ProcNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
  }
}
TEST(Gather_MPI, Test_IntVector3) {
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
    globalSize = gen() % 10 + 1;
    globalSize = globalSize * ProcNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / ProcNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
  }
}
TEST(Gather_MPI, Test_IntVector4) {
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
    globalSize = gen() % 2 + 1;
    globalSize = globalSize * ProcNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / ProcNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
  }
}
TEST(Gather_MPI, Test_IntVector5) {
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
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    EXPECT_TRUE(timeImb * 2 > timeOwn);
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
