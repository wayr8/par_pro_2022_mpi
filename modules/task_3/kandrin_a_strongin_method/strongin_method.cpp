// Copyright 2022 Kandrin Alexey
#include "../../../modules/task_3/kandrin_a_strongin_method/strongin_method.h"

#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>

// calculate the "m" estimate of the Lipschitz constant:
// M = max{1 <= i <= n}(abs((Z_{i} - Z_{i-1}) / (Y_{i} - Y_{i - 1}))
// m = {1 if M = 0, r*M if M > 0}, where r > 1 - parameter,
// Z_{i} = f(Y_{i})
double Calculate_m(Function&& f, const std::vector<double>& y, const double r) {
  assert(r > 1);

  double M = 0;
  for (int i = 1; i < y.size(); ++i) {
    const double zDif = f(y.at(i)) - f(y.at(i - 1));
    const double yDif = y.at(i) - y.at(i - 1);
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
int CalculateIndexOfMaxR(Function&& f, const std::vector<double>& y,
                         const double m) {
  std::pair<double, int> maxR_Index(-1, -1);
  for (int i = 1; i < y.size(); ++i) {
    const double yDif = y.at(i) - y.at(i - 1);
    const double zDif = f(y.at(i)) - f(y.at(i - 1));
    const double zSum = f(y.at(i)) + f(y.at(i - 1));
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
  std::vector<double> y = {a, b};

  const double r = 2.0;

  while (true) {
    std::sort(y.begin(), y.end());

    const double m = Calculate_m(std::forward<Function>(f), y, r);
    int i = CalculateIndexOfMaxR(std::forward<Function>(f), y, m);
    if (y.at(i) - y.at(i - 1) < epsilon) {
      return f(y.at(i));
    }
    double yn = y.at(i - 1) + (y.at(i) - y.at(i - 1)) / 2 +
                (f(y.at(i)) - f(y.at(i - 1))) / (2 * m);
    y.push_back(yn);
  }
}
