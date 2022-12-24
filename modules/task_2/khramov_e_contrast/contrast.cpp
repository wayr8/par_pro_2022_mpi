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

void printMatrix(std::vector<std::vector<int>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        printVector(matrix[i]);
    }
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

std::vector<std::vector<int>> getRandomMatrix(int w, int h) {
    std::vector<std::vector<int>> matrix(h);

    for (int i = 0; i < h; i++) {
        matrix[i] = getRandomVector(w);
    }

    return matrix;
}

float getContrastCoeff(int contrast) {
    return (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
}

int truncate(size_t value) {
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}


std::vector<std::vector<int>> getContrastedMatrixSequential(std::vector<std::vector<int>> matrix) {

    int h = matrix.size();
    int w = matrix[0].size();

    std::vector<std::vector<int>> result(h, std::vector<int>(w, 0));

    float coeff = getContrastCoeff(10);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int color = matrix[i][j];
            int newColor = truncate((int)(coeff * (color - 128) + 128));
            result[i][j] = newColor;
        }
    }

    return result;
}
