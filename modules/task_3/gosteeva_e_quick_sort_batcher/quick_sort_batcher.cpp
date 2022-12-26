  // Copyright 2022 Gosteeva Ekaterina

#include "quick_sort_batcher.h"

void swap(int a, int b) {
    if (b < a) {
        std::swap(a, b);
    }
}

void Shuf(std::vector<int> &vec, int left_bord, int right_bord) {
    auto half = (left_bord + right_bord) / 2;
    std::vector<int> tmp(vec.size());
    for (int i = left_bord, j = 0; i <= right_bord; i += 2, j++) {
        tmp.at(i) = vec.at(left_bord + j);
        tmp.at(i + 1) = vec.at(half + j + 1);
    }
    for (size_t i = 0; i < tmp.size(); i++) {
        vec.at(i) = tmp.at(i);
    }
}

void Share(std::vector<int> &vec, int left_bord, int right_bord) {
    int half = (left_bord + right_bord) / 2;
    std::vector<int> tmp(vec.size());
    for (int i = left_bord, j = 0; i <= right_bord; i += 2, j++) {
        tmp.at(left_bord + j) = vec.at(i);
        tmp.at(half + j + 1) = vec.at(i + 1);
    }
    for (size_t i = 0; i < tmp.size(); i++) {
        vec.at(i) = tmp.at(i);
    }
}

void BatcherMerge(std::vector<int> &vec, int left_bord, int right_bord) {
    if (right_bord == left_bord + 1) {
        swap(vec.at(left_bord), vec.at(right_bord));
    }
    if (right_bord > left_bord + 2) {
        Share(vec, left_bord, right_bord);
        int half = (left_bord + right_bord) / 2;
        BatcherMerge(vec, left_bord, half);
        BatcherMerge(vec, half + left_bord, right_bord);
        Shuf(vec, left_bord, right_bord);
        for (int i = left_bord + 1; i < right_bord; i += 2) {
            swap(vec.at(i), vec.at(i + 1));
        }
        int halfSize = (right_bord - left_bord + 1) / 2 - 1;
        for (int i = left_bord + 1; i + halfSize < right_bord; i++) {
            swap(vec.at(i), vec.at(i + halfSize));
        }
    }
}

std::vector<int> GetRandomVec(int vec_size) {
    std::random_device rand_dev;
    std::mt19937 random(rand_dev());
    std::vector<int> vec(vec_size);
    for (int i = 0; i < vec_size; i++) {
        vec.at(i) = random() % 1000;
    }
    return vec;
}

std::vector<int> Merge(std::vector<std::vector<int>> vec) {
    while (vec.size() > 1) {
        for (size_t i = 0; i < vec.size(); i++) {
            std::vector<int> tmp = vec.at(i);
            tmp.insert(tmp.end(), vec.at(i + 1).begin(), vec.at(i + 1).end());
            BatcherMerge(tmp, 0, tmp.size());
            vec.at(i) = tmp;
            vec.erase(vec.begin() + i);
        }
    }
    return vec.at(0);
}

void QuickSort(std::vector<int> &vec, int left_bord, int right_bord) {
    if (left_bord < right_bord) {
        int pivot = vec.at(left_bord);
        int p = left_bord;
        for (int i = left_bord + 1; i < right_bord; i++) {
            if (vec.at(i) < pivot) {
                p++;
                std::swap(vec.at(i), vec.at(p));
            }
        }
        std::swap(vec.at(left_bord), vec.at(p));
        QuickSort(vec, left_bord, p);
        QuickSort(vec, p + 1, right_bord);
    }
}

std::vector<int> QuickSortParrallel(const std::vector<int> vec) {
    int MPI_rank, MPI_size;
    MPI_Status MPI_status;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);
    int int_size = vec.size() / MPI_size;
    std::vector<int> loc_vec(int_size);
    std::vector<std::vector<int>> vec_buffer;
    std::vector<int> result(vec.size());

    MPI_Scatter(vec.data(), int_size, MPI_INT,
                loc_vec.data(), int_size, MPI_INT, 0, MPI_COMM_WORLD);
    QuickSort(loc_vec, 0, loc_vec.size());
    if (MPI_rank == 0) {
        vec_buffer.push_back(loc_vec);
        for (int i = 1; i < MPI_size; i++) {
            MPI_Recv(loc_vec.data(), int_size, MPI_INT, i, 0, MPI_COMM_WORLD, &MPI_status);
        }
        result = Merge(vec_buffer);
    } else {
        MPI_Send(loc_vec.data(), int_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return result;
}
