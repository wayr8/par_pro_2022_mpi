// Copyright 2022 Sukharev Artem
#include "../../modules/task_3/sukharev_a_gsa_parall_by_charact/glob_opt.h"

#include <mpi.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

constexpr double R = 20;
constexpr int K = 300;

void one_test(std::vector<double> x, std::function<double(double, double)> func,
              std::vector<test_point>& points, test_point& best) {
  double z = func(x[0], x[1]);
  points.push_back(test_point(x, z));
  if (best.z > z) {
    best = test_point(x, z);
  }
  std::cout << "x,z = " << x[1] << "," << z << "\n";
}

double calc_const_M(std::vector<test_point>& points, int dimension) {
  double miniM = 0;
  for (int i = 1; i < points.size(); i++) {
    miniM =
        std::max(miniM, std::abs(points[i].z - points[i - 1].z) /
                            (points[i][dimension] - points[i - 1][dimension]));
  }
  double M = 1;
  if (miniM > 0) {
    M = R * miniM;
  }
  return M;
}

std::vector<std::pair<double, int>> calc_characteristics(
    std::vector<test_point>& points, double M, int dimension) {
  std::vector<std::pair<double, int>> characteristics(points.size() - 1);
  for (int i = 0; i < points.size() - 1; i++) {
    double dX = points[i + 1][dimension] - points[i][dimension];
    double dZ = points[i + 1].z - points[i].z;
    double sZ = points[i + 1].z + points[i].z;
    characteristics[i] = {M * dX + dZ * dZ / (M * dX) - 2 * sZ, i};
  }
  return characteristics;
}

double one_dimension_gsa(std::function<double(double, double)> func, double x0,
                         double leftBorder, double rightBorder, double deltaX) {
  std::vector<test_point> points;
  test_point best;
  {
    one_test(std::vector<double>{x0, leftBorder}, func, points, best);
    best = points[0];
    one_test(std::vector<double>{x0, rightBorder}, func, points, best);
  }
  for (int j = 2; j < K; j++) {
    sort(points.begin(), points.end());
    double M = calc_const_M(points, 1);
    std::vector<std::pair<double, int>> characteristics =
        calc_characteristics(points, M, 1);
    sort(characteristics.rbegin(), characteristics.rend());
    int tmp_interval = characteristics[0].second;
    if (points[tmp_interval + 1][1] - points[tmp_interval][1] < deltaX) {
      break;
    }
    double newX1 =
        (points[tmp_interval + 1][1] + points[tmp_interval][1]) / 2 -
        (points[tmp_interval + 1].z - points[tmp_interval].z) / (2 * M);
    one_test(std::vector<double>{x0, newX1}, func, points, best);
  }
  return best.z;
}
