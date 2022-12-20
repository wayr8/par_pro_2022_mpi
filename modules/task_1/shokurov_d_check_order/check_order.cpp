// Copyright 2022 Shokurov Daniil
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/shokurov_d_check_order/check_order.h"
using namespace std;

void get_random_string(string& str, size_t n) {
    str.resize(n);
    for (int i = 0; i < n; ++i) {
        str[i] = (char)rand();
    }
};

void scatter_string(string& str1) {
    int rank;
    int numProc = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int count = str1.size();
        const char* ptr1 = str1.c_str();
        count += (numProc - (((count - 1) % numProc) + 1));
        str1.resize(count);
        for (int i = str1.size(); i < count; ++i) {
            str1[i] = 0;
        }
        int si = count / numProc;
        for (int i = 1; i < numProc; ++i) {
            MPI_Send(ptr1 + i * si, si, MPI_CHAR, i, 123, MPI_COMM_WORLD);
        }
        str1.resize(si);
    }
    else {
        MPI_Status status;
        char* buffer;
        int count;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &count);
        buffer = new char[count];
        MPI_Recv(buffer, count, MPI_CHAR, 0, 123, MPI_COMM_WORLD, &status);
        str1 = string(buffer, count);
    }
};

void get_string(string& str, int root) {
    MPI_Status status;
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    str.resize(count);
    char* ptr = const_cast<char*>(str.c_str());
    MPI_Scatter(nullptr, 0, MPI_CHAR, ptr, count, MPI_CHAR, root, MPI_COMM_WORLD);
}

int check_order_single_process(size_t n, string& a, string& b) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            if (a[i] < b[i]) return -1;
            if (a[i] > b[i]) return 1;
        }
    }
    return 0;
}

void addNull(string& str, int count) {
    int si = str.size();
    str.resize(str.size() + count);
    for (int i = si; i < str.size(); i++) {
        str[i] = 0;
    }
}

int getOrder(string& str1, string& str2) {
    int rank = 0;
    int numProc = 0;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (str1.size() < str2.size()) {
        addNull(str1, str2.size() - str1.size());
    }
    else {
        addNull(str2, str1.size() - str2.size());
    }

    scatter_string(str1);
    scatter_string(str2);

    int ans = check_order_single_process(str1.size(), str1, str2);

    if (rank == 0) {
        for (int i = 1; i < numProc; ++i) {
            int x;
            MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, 456, MPI_COMM_WORLD, &status);
            if (ans == 0)ans = x;
        }
    }
    else {
        MPI_Send(&ans, 1, MPI_INT, 0, 456, MPI_COMM_WORLD);
    }
    return ans;
}