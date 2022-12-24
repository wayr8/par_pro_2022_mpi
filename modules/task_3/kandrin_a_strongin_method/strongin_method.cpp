// Copyright 2022 Kandrin Alexey
#include "../../../modules/task_3/kandrin_a_strongin_method/strongin_method.h"

#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>


//=============================================================================
// Function : WorkSplitter::WorkSplitter
// Purpose  : Constructor.
//=============================================================================
WorkSplitter::WorkSplitter(size_t work, size_t workerCount)
    : m_workDistribution(workerCount, 0) {
  if (work <= workerCount) {
    for (size_t currentWorker = 0; currentWorker < work; ++currentWorker) {
      m_workDistribution[currentWorker] = 1;
    }
  } else {
    for (size_t currentWorker = 0; work != 0; ++currentWorker) {
      size_t workForCurrentWorker = work / workerCount;
      m_workDistribution[currentWorker] = work / workerCount;
      work -= workForCurrentWorker;
      workerCount -= 1;
    }
  }
}

//=============================================================================
// Function : GetPartWork
// Purpose  : Determining how much work a worker should do.
//=============================================================================
size_t WorkSplitter::GetPartWork(size_t workerNumber) const {
  return m_workDistribution[workerNumber];
}

size_t WorkSplitter::GetPrevPartWork(size_t workerNumber) const {
  size_t work = 0;
  for (size_t i = 0; i < workerNumber; ++i) {
    work += m_workDistribution.at(i);
  }
  return work;
}

// calculate the "m" estimate of the Lipschitz constant:
// M = max{1 <= i <= n}(abs((Z_{i}.end - Z_{i}.begin) / (Y_{i}.end -
// Y_{i}.begin)) m = {1 if M = 0, r*M if M > 0}, where r > 1 - parameter,
// Z_{i}.begin = f(Y_{i}.begin)
// Z_{i}.end = f(Y_{i}.end)
double Calculate_m(Function&& f, const std::vector<Segment>& y,
                   const double r) {
  assert(r > 1);

  double M = 0;
  for (int i = 0; i < y.size(); ++i) {
    const double y_begin = y.at(i).begin;
    const double y_end = y.at(i).end;

    const double zDif = f(y_end) - f(y_begin);
    const double yDif = y_end - y_begin;
    const double currentMax = std::abs(zDif / yDif);
    if (currentMax > M) {
      M = currentMax;
    }
  }

  return (M == 0 ? 1 : r * M);
}


// sequential version
template <>
std::pair<double, int> CalculateIndexOfMaxR<true>(Function&& f,
                                                  const std::vector<Segment>& y,
                                                  const double m) {
  std::pair<double, int> maxR_Index(-DBL_MAX, -1);

  for (int i = 0; i < y.size(); ++i) {
    const double y_begin = y.at(i).begin;
    const double y_end = y.at(i).end;
    Debug(i, " handle: ", y_begin, ' ', y_end, '\n');

    const double yDif = y_end - y_begin;
    const double zDif = f(y_end) - f(y_begin);
    const double zSum = f(y_end) + f(y_begin);
    const double R = m * yDif + zDif * zDif / (m * yDif) - 2 * zSum;

    double& maxR = maxR_Index.first;
    int& maxIndex = maxR_Index.second;

    if (R > maxR) {
      maxR = R;
      maxIndex = i;
    }
  }
  Debug("Handle result: ", maxR_Index.first, ' ', maxR_Index.second, "\n\n");
  return maxR_Index;
}

// parallel version
template <>
std::pair<double, int> CalculateIndexOfMaxR<false>(
    Function&& f, const std::vector<Segment>& y, const double m) {
  std::pair<double, int> maxR_Index(-DBL_MAX, -1);

  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  WorkSplitter workSplitter(y.size(), procCount);
  /*if (rank == 0) {
    for (size_t i = 0; i < procCount; ++i) {
      Debug("For proc ", i, " workCount = ", workSplitter.GetPartWork(i), "\n");
    }
  }*/

  size_t workForThisProc = workSplitter.GetPartWork(rank);
  std::vector<Segment> localY;

  if (rank == 0) {
    size_t distributedWork = workForThisProc;

    for (int procNum = 1; procNum < procCount; ++procNum) {
      size_t workForProc = workSplitter.GetPartWork(procNum);
      if (workForProc != 0) {
        MPI_Send(&y.at(distributedWork), workForProc * sizeof(Segment), MPI_CHAR,
                 procNum,
                 0, MPI_COMM_WORLD);
      }

      distributedWork += workForProc;
    }

    localY = std::vector<Segment>(y.begin(), y.begin() + workForThisProc);
  } else {
    if (workForThisProc != 0) {
      localY = std::vector<Segment>(workForThisProc);
      MPI_Recv(localY.data(), workForThisProc * sizeof(Segment), MPI_CHAR, 0,
               0,
               MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }
  }

  auto indexOfMaxR =
      CalculateIndexOfMaxR<true>(std::forward<Function>(f), localY, m);

  Debug(indexOfMaxR.first, ' ', indexOfMaxR.second, '\n');

  if (rank == 0) {
    std::vector<std::pair<double, int>> results(procCount, std::pair<double, int>(-DBL_MAX, -1));
    results.at(0) = indexOfMaxR;

    for (int procNum = 1; procNum < procCount; ++procNum) {
      if (workSplitter.GetPartWork(procNum) != 0) {
        MPI_Recv(&results.at(procNum), sizeof(indexOfMaxR), MPI_CHAR, procNum,
                 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }

    Debug("pairs:\n");
    for (const auto& pair : results) {
      Debug(pair.first, ' ', pair.second, '\n');
    }

    auto iter = std::max_element(
        results.begin(), results.end(),
        [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
          return a.first < b.first;
        });
    size_t indexOfMaxElement = iter - results.begin();
    results.at(indexOfMaxElement).second +=
        workSplitter.GetPrevPartWork(indexOfMaxElement);
    
    Debug("Max pair = ", iter->first, ' ', iter->second, '\n');
    return *iter;

  } else {
    if (workForThisProc != 0) {
      MPI_Send(&indexOfMaxR, sizeof(indexOfMaxR), MPI_CHAR, 0, 0,
               MPI_COMM_WORLD);
    }
  }

  return {};
}
