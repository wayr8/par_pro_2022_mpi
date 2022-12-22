// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./sleeping_barber.h"
#include <fstream>
#include <thread>
#include <chrono>


extern std::ofstream out_room;
extern std::ofstream out_client;
extern std::ofstream out_barber;

TEST(Parallel_Operations_MPI, Test_Barber_Shop_Work) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 1;

  if (size >= 3) {
    if (rank == barberProc) {
      int served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      } else {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "End" << std::endl;
    } else {
      std::this_thread::sleep_for(
          std::chrono::duration<double, std::milli>(10));
      customer();
      out_client << "end." << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_Barber_Shop_Work_2) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 3;

  if (size >= 3) {
    if (rank == barberProc) {
      int served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      } else {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "End" << std::endl;
    } else {
      std::this_thread::sleep_for(
          std::chrono::duration<double, std::milli>(10));
      customer();
      out_client << "end." << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_Barber_Shop_Work_3) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 5;

  if (size >= 3) {
    if (rank == barberProc) {
      int served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      } else {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "End" << std::endl;
    } else {
      std::this_thread::sleep_for(
          std::chrono::duration<double, std::milli>(10));
      customer();
      out_client << "end." << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_Barber_Shop_Work_4) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 6;

  if (size >= 3) {
    if (rank == barberProc) {
      int served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      } else {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "End" << std::endl;
    } else {
      std::this_thread::sleep_for(
          std::chrono::duration<double, std::milli>(10));
      customer();
      out_client << "end." << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_Barber_Shop_Work_5) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int waitingRoomSize = 10;

  if (size >= 3) {
    if (rank == barberProc) {
      int served_cust = barber();
      out_barber << "end." << std::endl;
      if (size - 2 <= waitingRoomSize) {
        ASSERT_TRUE(served_cust == size - 2);
      } else {
        ASSERT_TRUE(served_cust >= waitingRoomSize);
      }
    } else if (rank == waitingRoomProc) {
      waitingRoom(waitingRoomSize);
      out_room << "End" << std::endl;
    } else {
      std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(10));
      customer();
      out_client << "end." << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

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
