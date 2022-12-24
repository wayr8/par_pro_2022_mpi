// Copyright 2022 Kandrin Alexey
#ifndef MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
#define MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_

#include <vector>
#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>



#include <string>
#include <fstream>
#include <iostream>
template <class... Args>
static void Debug(Args... args) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::ofstream out("output" + std::to_string(rank) + ".txt",
                    std::ios_base::app);
  int dummy[] = {(out << args, 0)...};
}


//=============================================================================
// Class   : WorkSplitter
// Purpose : Calculating how much work the worker needs to do.
//=============================================================================
class WorkSplitter {
  std::vector<size_t> m_workDistribution;

 public:
  // Constructor.
  // \param work Total amount of work.
  // \param workerCount Total number of workers who can do the work.
  WorkSplitter(size_t work, size_t workerCount);

  // Determining how much work a worker should do.
  // \param workerNumber The worker for whom we calculate how much work he has
  // to do. \return What part of the work should worker do. (If there are fewer
  // work than workerCount, then some workers will do nothing. If work is
  // proportional to the workerCount, then all workers will do the same amount
  // of work)
  size_t GetPartWork(size_t workerNumber) const;

  size_t GetPrevPartWork(size_t workerNumber) const;
};

using Function = double (*)(double);

struct Segment {
  double begin;
  double end;
};

double Calculate_m(Function&& f, const std::vector<Segment>& y, const double r);

// Calculate all of characteristic "R".
// R(i) = m(Y_{i} - Y_{i - 1}) + sqr(Z_{i} - Z_{i - 1}) / m(Y_{i} - Y{i - 1}) -
// 2(Z_{i} + Z_{i-1}), where m = Calculate_M(...) (see above),
// Z_{i} = f(Y_{i})
template <bool isSequential>
std::pair<double, int> CalculateIndexOfMaxR(Function&& f,
                                            const std::vector<Segment>& y,
                                            const double m);





// Get minimum of function f in [a; b]
static constexpr bool sequential = true;
static constexpr bool parallel = false;

template<bool ExecutionPolicy>
double GetMin(Function&& f, double a, double b, double epsilon) {
  std::vector<Segment> y = {Segment{a, b}};

  const double r = 2.0;
  const size_t maxIterationCount = 100000;

  for (size_t iterationIndex = 0; iterationIndex < maxIterationCount;
       ++iterationIndex) {
    Debug("________________\nIteration index: ", iterationIndex, "\n");
    const double m = Calculate_m(std::forward<Function>(f), y, r);
    auto indexOfMaxR =
        CalculateIndexOfMaxR<ExecutionPolicy>(std::forward<Function>(f), y, m);

    MPI_Bcast(&indexOfMaxR, sizeof(indexOfMaxR), MPI_CHAR, 0, MPI_COMM_WORLD);
    Debug("Current indexOfMaxR = ", indexOfMaxR.first, ' ', indexOfMaxR.second,
          '\n');
    const auto& currentSegment = y.at(indexOfMaxR.second);
    const double y_begin = currentSegment.begin;
    const double y_end = currentSegment.end;
    if (y_end - y_begin < epsilon) {
      return f(y_end);
    }
    double yn =
        y_begin + (y_end - y_begin) / 2 + (f(y_end) - f(y_begin)) / (2 * m);
    y.push_back(Segment{y_begin, yn});
    y.at(indexOfMaxR.second).begin = yn;
    Debug("Segments: ");
    for (const auto & segment : y) {
      Debug(segment.begin, ' ', segment.end, "; ");
    }
    Debug("\n");
  }

  // calculation error
  return NAN;
}

#endif  // MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
