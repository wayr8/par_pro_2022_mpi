// Copyright 2022 Bataev Ivan
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include "./vector_alternations_signs.h"

std::vector<int> getRandomVector(int size, int left, int right) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> distr(left, right);
	std::vector<int> v(size);
	for (auto &elem : v) { elem = distr(mersenne); }
	return v;
}

void printVector(const std::vector<int>& v, std::string prefix) {
	std::cout << prefix << "[ ";
	for (const auto& elem : v) { std::cout << elem << " "; }
	std::cout << "]\n";
}

int getNumAlterSignsSequential(std::vector<int> v) {
	int count = 0;
	for (int i = 0; i < v.size() - 1; i++)
		if (v[i] > 0 && v[i + 1] < 0 || v[i] < 0 && v[i + 1] > 0) // 0 doesn't count as a sign change
			count++;
	return count;
}

int getNumAlterSignsParallel(std::vector<int> gv) {
	int commSize, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int gvSize = int(gv.size());
	const int lvSize = gvSize / commSize + 1; // base size of a local vector (also take one more element to count at segment borders)
	const int restSize = gvSize % commSize; // number of elements not included in any local vector of base size

	bool fix = (commSize - rank <= restSize); // distribute starting from the last rank
	const int _lvSize = lvSize + fix; // fixed size of a local vector 

	// splitting the original vector to segments
	if (rank == 0) {
		int shift = lvSize - 1; // '-1' because one more element was taken (39 line)
		for (int _rank = 1; _rank < commSize - 1; _rank++) {
			bool fix = (commSize - _rank <= restSize);
			int _lvSize = lvSize + fix;
			MPI_Send(gv.data() + shift, _lvSize, MPI_INT, _rank, 0, MPI_COMM_WORLD);
			shift += _lvSize - 1;
		}
		if (commSize != 1) {
			bool fix = (restSize != 0);
			int _lvSize = lvSize + fix - 1; // '-1' because one more element was taken (39 line) and this rank is the last
			MPI_Send(gv.data() + shift, _lvSize, MPI_INT, (commSize - 1), 0, MPI_COMM_WORLD);
		}
	}

	// local vector initialization
	std::vector<int> lv;
	if (rank == (commSize - 1))
		lv = std::vector<int>(_lvSize - 1);
	else
		lv = std::vector<int>(_lvSize);

	if (commSize == 1)
		lv = std::vector<int>(gv.begin(), gv.end());
	else {
		if (rank == 0)
			lv = std::vector<int>(gv.begin(), gv.begin() + _lvSize);
		else if (rank == (commSize - 1)) {
			MPI_Status status;
			MPI_Recv(lv.data(), _lvSize - 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		}
		else {
			MPI_Status status;
			MPI_Recv(lv.data(), _lvSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		}
	}

	// counting
	int gv_count = 0;
	int lv_count = getNumAlterSignsSequential(lv);
	//printVector(lv, std::to_string(rank) + ": ");
	MPI_Reduce(&lv_count, &gv_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	return gv_count;
}
