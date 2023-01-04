// Copyright 2022 Sukharev Artem
#ifndef MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GSA_PARALL_BY_CHARACT_H_
#define MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GSA_PARALL_BY_CHARACT_H_

#include <algorithm>
#include <functional>
#include <vector>

struct test_point {
  std::vector<double> x;
  double z;
  test_point(std::vector<double> _x = {0, 0}, double _z = 0) : x(_x), z(_z) {}
  bool operator<(const test_point a) const {
    if (x != a.x) return x < a.x;
    return z < a.z;
  }
  double operator[](const int a) const { return x[a]; }
};

double one_dimension_gsa(std::function<double(double, double)> func, double x0,
                         double leftBorder = 0.0, double rightBorder = 1.0,
                         double deltaX = 0.001);

#endif  // MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GSA_PARALL_BY_CHARACT_H_
