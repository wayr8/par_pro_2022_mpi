// Copyright 2023 Yunin Daniil
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/yunin_d_convex_shell_for_components_binary_image/convex_shell_for_components_binary_image.h"

using std::vector;

/*
    Пример изображения
(0,0) - 0 0 0 0 0 0 0
        0 0 1 0 0 0 0
        0 1 1 1 0 0 0
        0 1 1 0 0 0 0
        0 0 0 0 0 1 0
        0 0 0 1 1 1 0
        0 0 0 0 0 0 0 - (6,6) координаты   
*/

void CreateImage1(vector<int> &image) {
    image[1*7+2] = 1;
    image[2*7+1] = 1; image[2*7+2] = 1; image[2*7+3] = 1;
    image[3*7+1] = 1; image[3*7+2] = 1;
    image[4*7+5] = 1;
    image[5*7+3] = 1; image[5*7+4] = 1; image[5*7+5] = 1;
}

void PrintImage(vector<int> &image, int width, int height) {
    std::cout << "\tИзображение\n";
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << image[i*width+j] << ' ';
        }
        std::cout << std::endl;
    }
}

void JarvisMarch(vector<int> &image, int width, int height) {
    // Берём самую левую точку из изображения
    vector<int> convex_hull;
    int x = width, y = 0; // координаты самой левой нижней точки
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << image[i*width+j] << " "; 
            if ((image[i*width+j] == 1) && (j < x)) {
                x = j;
                y = i;
            }
        }
    }
    convex_hull.push_back(x);
    convex_hull.push_back(y);
    std::cout << convex_hull[0] << ' ' << convex_hull[1] << std::endl << image[y*width+x];
}


// std::vector<int> getRandomVector(int sz) {
//     std::random_device dev;
//     std::mt19937 gen(dev());
//     std::vector<int> vec(sz);
//     for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
//     return vec;
// }

// int getSequentialOperations(std::vector<int> vec, const std::string& ops) {
//     const int  sz = vec.size();
//     int reduction_elem = 0;
//     if (ops == "+") {
//         for (int  i = 0; i < sz; i++) {
//             reduction_elem += vec[i];
//         }
//     } else if (ops == "-") {
//         for (int  i = 0; i < sz; i++) {
//             reduction_elem -= vec[i];
//         }
//     } else if (ops == "max") {
//         reduction_elem = vec[0];
//         for (int  i = 1; i < sz; i++) {
//             reduction_elem = std::max(reduction_elem, vec[i]);
//         }
//     }
//     return reduction_elem;
// }

// int getParallelOperations(std::vector<int> global_vec,
//                           int count_size_vector, const std::string& ops) {
//     int size, rank;
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     const int delta = count_size_vector / size;

//     if (rank == 0) {
//         for (int proc = 1; proc < size; proc++) {
//             MPI_Send(global_vec.data() + proc * delta, delta,
//                         MPI_INT, proc, 0, MPI_COMM_WORLD);
//         }
//     }

//     std::vector<int> local_vec(delta);
//     if (rank == 0) {
//         local_vec = std::vector<int>(global_vec.begin(),
//                                      global_vec.begin() + delta);
//     } else {
//         MPI_Status status;
//         MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//     }

//     int global_sum = 0;
//     int local_sum = getSequentialOperations(local_vec, ops);
//     MPI_Op op_code = MPI_OP_NULL;
//     if (ops == "+" || ops == "-") { op_code = MPI_SUM; }
//     if (ops == "max") { op_code = MPI_MAX; }
//     MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
//     return global_sum;
// }
