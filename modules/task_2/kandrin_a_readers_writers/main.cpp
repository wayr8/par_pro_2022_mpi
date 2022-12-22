// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./readers_writers.h"

#ifdef DEBUG_OUTPUT
#include <fstream>
std::vector<std::ofstream> outs;
#endif

TEST(Parallel_Operations_MPI, Test_one_reader_and_other_writers) {
  int processCount;
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);
  if (processCount < 3) {
    // this test requires a minimum of 3 processes: master process, 1 reader and
    // 1 writer
    ASSERT_TRUE(true);
    return;
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const bool isMasterProcess = rank == 0;
  const bool isReaderProcess = rank == 1;
  const bool isWriterProcess = rank > 1;
  const int writersCount = processCount - 2;

  const int startValue = 0;
  const int index = 0;
  if (isMasterProcess) {
    auto memory = std::make_unique<Memory>();
    ASSERT_NE(memory, nullptr);
    masterProcessFunction(memory.get());
    ASSERT_EQ(
        *reinterpret_cast<const int*>(memory->Read(sizeof(int), 0).GetData()),
        0);
    outs[rank] << "End function\n";
  } else if (isWriterProcess) {
    std::vector<OperationInt> operations = {
        OperationInt(index, OperationInt::OperationType::operator_add, 1),
        OperationInt(index, OperationInt::OperationType::operator_dif, 1)};
    writerProcessFunction(operations);
  } else if (isReaderProcess) {
    auto readResult = readerProcessFunction(writersCount);
    for (auto& value : readResult) {
      EXPECT_GT(value, -1 * writersCount);
      EXPECT_LT(value, 1 * writersCount);
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

#ifdef DEBUG_OUTPUT
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  outs = std::vector<std::ofstream>(size);
  for (int i = 0; i < size; ++i) {
    outs[i] = std::ofstream(std::string("output") + std::to_string(i) + ".txt",
                            std::ios_base::out);
  }
#endif

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
