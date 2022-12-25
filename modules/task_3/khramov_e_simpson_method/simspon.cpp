// Copyright 2022 Khramov Egor

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <functional>
#include <vector>
#include "../../../modules/task_3/khramov_e_simpson_method/simpson.h"

template <typename T>
void printVector(std::vector<T> vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void printArray(T* arr, int n) {
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

double getRectangleValue(function f, double x, double y, double z, double* h) {
    return f(x + h[0]/2, y + h[1]/2, z + h[2]/2);
}

double getSimpsonValue(function f, double x, double y, double z, double* h) {
    return (f(x, y, z) + 3 * f(x + h[0] / 3, y + h[1] / 3, z + h[2] / 3) + 3 * f(x + 2 * h[0] / 3, y + 2 * h[1] / 3, z + 2 * h[2] / 3) + f(x + h[0], y + h[1], z + h[2])) / 8;
}

double integrateSequential(function f, double* a, double* b, double* n) {
    double result = 0.0;

    double h[3];
    for (int i = 0; i < 3; i++) {
        h[i] = (b[i] - a[i]) / n[i];
    }

    for (double x = a[0]; x <= b[0]; x += h[0]) {
        for (double y = a[1]; y <= b[1]; y += h[1]) {
            for (double z = a[2]; z <= b[2]; z += h[2]) {
                double value = getSimpsonValue(f, x, y, z, h);
                // double value = getRectangleValue(f, x, y, z, h);
                result += h[0] * h[1] * h[2] * value;
            }
        }
    }

    return result;
}

double integrateParallel(function f, double* a, double* b, double* n) {
    double result = 0.0;

    int comm_size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << "Process number " << rank << std::endl; 

    return result;
}
