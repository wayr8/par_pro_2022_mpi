// Copyright 2022 Yunin D.
#include "../../../modules/task_1/yunin_d_vector_order_errors/vector_order_errors.h"
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>

using std::vector;
using std::mt19937;
using std::random_device;

// функция принимает 2 значения - размер массива, верхнюю границу чисел и смещение для указание диапазона;
vector<int> CreateRandomVector(int v_size, int right_border, int offset) {
    vector<int> my_vector(v_size);
    mt19937 gen;
    UpdateRandNumbers(&gen);
    for (int i = 0; i < my_vector.size(); i++) {
        my_vector[i] = gen() % right_border + offset;
    }
    return my_vector;
}

// В качестве порядка возьмём от меньшего к большему
int CountErrorsOrderNeigboringElementsVector(const vector<int> &my_vector) {
    int num_errors = 0;
    for (int i = 1; i < my_vector.size(); i++) {
        if (my_vector[i-1] > my_vector[i]) {
            num_errors++;
        }
    }
    return num_errors;
}

int CountErrorsOrderNeigboringElementsVectorParallel(const vector<int> &my_vector) {
    int num_errors = 0, local_num_errors = 0;
    int size, current_rank, part;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    part = my_vector.size() / size;
    vector<int> local_vec(part*2);  // такого числа точно хватит, больше быть не может
    // дробим количество элементом на число процессов
    if (current_rank == 0) {
        // занимаемся рассылкой данных процессам
        for (int i = 1; i < size; i++) {
            MPI_Send(my_vector.data() + (i-1) * part, part + 1, MPI_INT, i, 7, MPI_COMM_WORLD);
        }
        // остатки данных кладём в 0 процесс
        local_vec = vector<int>(my_vector.begin() + part * (size - 1), my_vector.end());
    } else {
        // перевыделяем память, так как тут конкретное число придёт и получаем данные
        local_vec.resize(part + 1);
        MPI_Recv(local_vec.data(), part + 1, MPI_INT, 0, 7, MPI_COMM_WORLD, &status);
    }
    // для каждого локального вектора считаем число ошибок
    for (int i = 1; i < local_vec.size(); i++) {
        if (local_vec[i-1] > local_vec[i]) {
            local_num_errors++;
        }
    }
    // собираем все данные
    MPI_Reduce(&local_num_errors, &num_errors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return num_errors;
}

void UpdateRandNumbers(mt19937 *gen) {
    random_device rd;
    (*gen).seed(rd());
}
