// Copyright 2022 Khramov Egor

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "../../../modules/task_3/khramov_e_simpson_method/simpson.h"

void printVector(std::vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 255;
    }
    return vec;
}

std::vector<int> getRandomMatrix(int w, int h) {
    return getRandomVector(w * h);
}
