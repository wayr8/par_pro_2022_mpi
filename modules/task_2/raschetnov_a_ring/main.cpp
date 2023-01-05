// Copyright 2023 Raschetnov Alexei
#include <gtest/gtest.h>
#include <utility>
#include "./ring.h"
#include <gtest-mpi-listener.hpp>

// void TestTemplate(std::pair<int, int> a, std::pair<int, int> b) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);
//   int information, delta, root;
//   int par_result;

//   if (rank == 0) {
//     information = getRandomNumber(a.first, a.second);
//     delta = getRandomNumber(b.first, b.second);
//     root = getRandomNumber(0, size - 1);
//   }

//   MPI_Bcast(&information, 1, MPI_INT, 0, MPI_COMM_WORLD);
//   MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
//   MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

//   par_result = SendRingParallel(information, 1, MPI_INT, root, delta, 0, MPI_COMM_WORLD);
//   if (rank == root) {
//     int seq_result = getResult(information, size, delta);
//     ASSERT_EQ(par_result, seq_result);
//   }
// }

// TEST(Parallel_Operations_MPI, test_1) {
//   std::pair<int, int> p1{10, 100};
//   std::pair<int, int> p2{2, 5};
//   TestTemplate(p1, p2);
// }

// TEST(Parallel_Operations_MPI, test_2) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);

//   int information = 0;
//   int delta = 2;
//   int root = 0;
//   int par_result;

//   // if (rank == 0) {
//   //   information = getRandomNumber(a.first, a.second);
//   //   delta = getRandomNumber(b.first, b.second);
//   //   root = getRandomNumber(0, size - 1);
//   // }

//   // MPI_Bcast(&information, 1, MPI_INT, 0, MPI_COMM_WORLD);
//   // MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
//   // MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

//   par_result = SendRingParallel(information, 1, MPI_INT, root, delta, 0, MPI_COMM_WORLD);
//   if (rank == 0) {
//     int seq_result = getResult(0, size, 2);
//     ASSERT_EQ(par_result, seq_result);
//   }
// }


TEST(Parallel_Operations_MPI, test_2) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // std::pair<int, int> p1{33, 44};
  // std::pair<int, int> p2{16, 21};
  // TestTemplate(p1, p2);
  int root, destination;
  int par_result;

  if (rank == 0) {
    root = getRandomNumber(0, size - 1);
    destination = getRandomNumber(0, size - 1);

    // std::cout << "Root: " << root << std::endl;
    // std::cout << "Destination: " << destination << std::endl;
  }
  MPI_Bcast(&destination, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  par_result = SendRingParallel(2, 1, MPI_INT, root, destination, 0, MPI_COMM_WORLD);
  //if (rank == root)
    
  if (rank == root) {
    int seq_result = 2 + 1 * constructPath(root, destination, size).size() - 1;
    ASSERT_EQ(par_result, seq_result);    
  }
}

// TEST(Parallel_Operations_MPI, test_3) {
//   std::pair<int, int> p1{-2, 14};
//   std::pair<int, int> p2{-10, 0};
//   TestTemplate(p1, p2);
// }

// TEST(Parallel_Operations_MPI, test_4) {
//   std::pair<int, int> p1{99, 153};
//   std::pair<int, int> p2{40, 45};
//   TestTemplate(p1, p2);
// }

// TEST(Parallel_Operations_MPI, test_5) {
//   std::pair<int, int> p1{1, 10};
//   std::pair<int, int> p2{1, 3};
//   TestTemplate(p1, p2);
// }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
