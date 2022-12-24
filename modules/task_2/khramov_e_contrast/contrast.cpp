// Copyright 2022 Khramov Egor

#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>

void printVector(std::vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " ";
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

float getContrastCoeff(int contrast) {
    return (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
}

int truncate(size_t value) {
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}


std::vector<int> getContrastedMatrixSequential(std::vector<int> matrix) {

    int length = matrix.size();

    std::vector<int> result(length);

    float coeff = getContrastCoeff(10);

    for (int i = 0; i < length; i++) {
        int color = matrix[i];
        int newColor = truncate((int)(coeff * (color - 128) + 128));
        result[i] = newColor;
    }

    return result;
}
