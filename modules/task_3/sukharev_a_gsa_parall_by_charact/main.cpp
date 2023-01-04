// Copyright 2022 Sukharev Artem
#include <gtest/gtest.h>

#include <functional>
#include <iostream>

#include <gtest-mpi-listener.hpp>
#include "./glob_opt.h"

TEST(Glob_opt_parall_by_characteristics, Test_1) {
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    // return sin(x)+cos(y); // -1 по y[0,10](x==0)
    // return sin(x)+y*sin(y*2+2)-cos(2*y); //-6.7306 по y[-3,10](x==0)
    return sin(x) + sin(y * 20 + 2) / y - 5 * y * cos(3 * y + 10); // -5.21608 по y[0.25,2.5](x==0)
  };
  double lbX = 0, rbX = 10;
  double lbY = 0.25, rbY = 2.5;
  std::cout << one_dimension_gsa(func, 0, lbY, rbY, 0.001) << "\n";
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
