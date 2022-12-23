#include <iostream>
#include <mpi.h>
#include <random>
#include "./vector_max.h"


void printVector(std::vector<int> vec) {
    for (int i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";
    std::cout << std::endl;
}

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

int getMax(std::vector<int> vec) {
    int max = vec[0];
    for (int i = 1; i < vec.size(); ++i)
        if (vec[i] > max)
            max = vec[i];
    return max;
}

int getMaxParallel(std::vector<double> globalVector, int global_vector_size) {
    return 1;
}




