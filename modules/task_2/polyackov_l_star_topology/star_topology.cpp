// Copyright 2022 Polyackov Lev
#include "../../../modules/task_2/polyackov_l_star_topology/star_topology.h"

void MPI_Star_create(MPI_Comm* comm) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> index;
    index.resize(size);
    int it = 0;
    for (int& tmp : index) {
        tmp = size - 1 + it;
        it++;
    }

    std::vector<int> edges;
    edges.resize(2 * (size - 1));
    std::fill(edges.begin(), edges.end(), 0);
    for (int i = 0; i < size - 1; i++) {
        edges[i] = i + 1;
    }
    if (size == 1) {
        edges.push_back(0);
    }

    MPI_Graph_create(MPI_COMM_WORLD, size, index.data(), edges.data(), false, comm);
}
