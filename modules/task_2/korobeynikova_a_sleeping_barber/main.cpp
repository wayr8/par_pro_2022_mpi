// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./sleeping_barber.h"
#include <fstream>


extern std::ofstream out_room;
extern std::ofstream out_client;
extern std::ofstream out_barber;

TEST(Parallel_Operations_MPI, Test_Max) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int  served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      }
      else
      {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "end." << std::endl;
    } else {
      customer();
      out_client << "end." << std::endl;
    }
  }
}
/*
TEST(Parallel_Operations_MPI, Test_Max_2) {
   int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int  served_cust = barber();
      ASSERT_TRUE(served_cust > waitingRoomSize);
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
    } else {
      customer();
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Max_3) {
    int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int  served_cust = barber();
      ASSERT_TRUE(served_cust > waitingRoomSize);
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
    } else {
      customer();
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Max_4) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int  served_cust = barber();
      ASSERT_TRUE(served_cust > waitingRoomSize);
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
    } else {
      customer();
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Max_5) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int  served_cust = barber();
      ASSERT_TRUE(served_cust > waitingRoomSize);
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
    } else {
      customer();
    }
  }
}
*/
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
