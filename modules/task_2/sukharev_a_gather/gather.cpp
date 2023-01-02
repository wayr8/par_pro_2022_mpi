// Copyright 2022 Sukharev Artem

#include "./gather.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <vector>


std::vector<int> getRandomVectorInt(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = gen()%1000;
  }
  return randomVector;
}
std::vector<float> getRandomVectorFloat(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<float> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = static_cast<float>(gen());
  }
  return randomVector;
}
std::vector<double> getRandomVectorDouble(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<double> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = static_cast<double>(gen());
  }
  return randomVector;
}

