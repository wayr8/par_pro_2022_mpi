// Copyright 2022 Kandrin Alexey
#include "../../../modules/task_3/kandrin_a_strongin_method/strongin_method.h"

#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>

namespace {
struct Segment {
  double begin;
  double end;
};
}

// calculate the "m" estimate of the Lipschitz constant:
// M = max{1 <= i <= n}(abs((Z_{i}.end - Z_{i}.begin) / (Y_{i}.end - Y_{i}.begin))
// m = {1 if M = 0, r*M if M > 0}, where r > 1 - parameter,
// Z_{i}.begin = f(Y_{i}.begin)
// Z_{i}.end = f(Y_{i}.end)
double Calculate_m(Function&& f, const std::vector<Segment>& y, const double r) {
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

// Calculate all of characteristic "R".
// R(i) = m(Y_{i} - Y_{i - 1}) + sqr(Z_{i} - Z_{i - 1}) / m(Y_{i} - Y{i - 1}) -
// 2(Z_{i} + Z_{i-1}), where m = Calculate_M(...) (see above),
// Z_{i} = f(Y_{i})
int CalculateIndexOfMaxR(Function&& f, const std::vector<Segment>& y,
                         const double m) {
  std::pair<double, int> maxR_Index(-1, -1);

  for (int i = 0; i < y.size(); ++i) {
    const double y_begin = y.at(i).begin;
    const double y_end = y.at(i).end;

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

  return maxR_Index.second;
}

// Get minimum of function f in [a; b]
double GetMin(Function&& f, double a, double b, double epsilon) {
  std::vector<Segment> y = {Segment{a, b}};

  const double r = 2.0;

  while (true) {

    const double m = Calculate_m(std::forward<Function>(f), y, r);
    int i = CalculateIndexOfMaxR(std::forward<Function>(f), y, m);
    const auto& currentSegment = y.at(i);
    const double y_begin = currentSegment.begin;
    const double y_end = currentSegment.end;
    if (y_end - y_begin < epsilon) {
      return f(y_end);
    }
    double yn =
        y_begin + (y_end - y_begin) / 2 + (f(y_end) - f(y_begin)) / (2 * m);
    y.push_back(Segment{y_begin, yn});
    y.at(i).begin = yn;
  }
}
