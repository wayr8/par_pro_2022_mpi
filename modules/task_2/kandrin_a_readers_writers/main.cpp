// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./readers_writers.h"

#ifdef DEBUG_OUTPUT
#include <fstream>
std::vector<std::ofstream> outs;
#endif

enum class ReturnValue {
    NOT_ENOUGH_PROCESSES,
    NOT_ENOUGH_MEMORY,
    SUCCESS,
    FAIL
};

static ReturnValue testFunction(int readersCount, int writersCount,
                         std::vector<OperationInt>& writeOperations, int expectedResult) {
  int processCount;
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

  if (processCount < 1 + readersCount + writersCount) {
    // this test requires a minimum of 1 + readersCount + writersCount
    // processes: master process, 2 readers and 1 writer
    return ReturnValue::NOT_ENOUGH_PROCESSES;
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  outs[rank] << "in test function\n";

  const bool isMasterProcess = rank == 0;
  const bool isReaderProcess = rank >= 1 && rank <= readersCount;
  const bool isWriterProcess = !(isMasterProcess || isReaderProcess);
  
  const int readingsCount = writersCount;

  const int startValue = 0;
  const int index = 0;

  if (isMasterProcess) {
    auto memory = std::make_unique<Memory>();
    if (memory == nullptr) {
      return ReturnValue::NOT_ENOUGH_MEMORY;
    }
    const int requestCount =
        readingsCount * readersCount + writeOperations.size() * writersCount;
    outs[rank] << "Request count:" << requestCount
               << "\nReadings count:" << readingsCount
               << "\nReaders count:" << readersCount
               << "\nWriters count:" << writersCount << std::endl;
    masterProcessFunction(memory.get(), requestCount);
    if (int(memory->Read(sizeof(int), 0)) == expectedResult) {
      outs[rank] << "End function\n";
      return ReturnValue::SUCCESS;
    } else {
      std::cout << "Expected: " << expectedResult << "\n";
      std::cout << "Actual: " << int(memory->Read(sizeof(int), 0)) << "\n";
    }
  } else if (isWriterProcess) {
    writerProcessFunction(writeOperations);
  } else if (isReaderProcess) {
    outs[rank] << "i read " << readingsCount << "times\n";
    auto readResult = readerProcessFunction(readingsCount);

    int minValue = INT_MAX;
    int maxValue = INT_MIN;
    for (const auto & operation : writeOperations) {
      int currentValue = 0;
      auto opType = operation.GetOperationType();
      if (opType == OperationInt::OperationType::operator_add) {
        currentValue = operation.GetArgument();
      } else if (opType == OperationInt::OperationType::operator_dif) {
        currentValue = -operation.GetArgument();
      }
      if (opType != OperationInt::OperationType::read) {
        if (currentValue > maxValue) {
          maxValue = currentValue;
        }
        if (currentValue < minValue) {
          minValue = currentValue;
        }
      }
    }

    bool isOk = true;
    for (auto& value : readResult) {
      isOk = (isOk && (value <= maxValue * writersCount));
      isOk = (isOk && (value >= minValue * writersCount));
    }
    if (!isOk) {
      return ReturnValue::FAIL;
    }
  }
  return ReturnValue::SUCCESS;
}

TEST(Parallel_Operations_MPI, Test_one_reader_and_other_writers) {
  int processCount;
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

  const int readersCount = 1;
  const int writersCount = processCount - readersCount - 1;
  if (writersCount < 1) {
    return;
  }

  std::vector<OperationInt> writeOperations = {
      OperationInt(0, OperationInt::OperationType::operator_add, 1),
      OperationInt(0, OperationInt::OperationType::operator_dif, 1)};

  auto res =
      testFunction(readersCount, writersCount, writeOperations, (1 - 1) * writersCount);
  ASSERT_EQ(res, ReturnValue::SUCCESS);
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_two_readers_and_other_writers) {
  int processCount;
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

  const int readersCount = 2;
  const int writersCount = processCount - readersCount - 1;
  if (writersCount < 1) {
    return;
  }

  std::vector<OperationInt> writeOperations = {
      OperationInt(0, OperationInt::OperationType::operator_add, 1),
      OperationInt(0, OperationInt::OperationType::operator_dif, 2)};

  auto res = testFunction(readersCount, writersCount, writeOperations, (1 - 2) * writersCount);
  ASSERT_EQ(res, ReturnValue::SUCCESS);
  MPI_Barrier(MPI_COMM_WORLD);
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
