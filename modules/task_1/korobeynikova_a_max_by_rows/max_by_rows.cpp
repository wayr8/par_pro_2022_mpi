// Copyright 2022 Korobeynikova Alisa
#include <mpi.h>
#include <random>
#include <algorithm>
#include "./max_by_rows.h"
#include <fstream>
#include <iostream>


#include <vector>
#include <sstream>
#include <iterator>

Matrix<int> getRandomMatrix(int n, int m) {
    std::random_device dev;
    std::mt19937 gen(dev());
    Matrix<int> matr(n, m);
    for (int  i = 0; i < n; i++) 
    {
        for (int j = 0; j < m; ++j)
        {
            matr[i][j] = gen() % 100;
        }
    }
    return matr;
}

int findMax(std::vector<int> vec) {
    const int  sz = vec.size();
    int max = 0;
    
    max = vec[0];
    for (int  i = 1; i < sz; i++) {
        max = std::max(max, vec[i]);
    }       
    return max;
}

std::string vtos(std::vector<int> v)
{
    std::ostringstream oss;
    if (!v.empty()) {
        std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(oss, ", "));
        oss << v.back();
    }
    return oss.str();
}

std::vector<int> getParallelOperation(Matrix<int> global_matr)
{
    std::ofstream outf("C:/Users/User/Desktop/process_input.txt", std::ios_base::app);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int rows_number = global_matr.GetN();
    const int cols_number = global_matr.GetM();
    const int rows_per_process = rows_number > size ? rows_number / size : 1;
    std::vector<int> max_values(rows_number);

    if (rank == 0) {
        int remained_rows = rows_number > size ? rows_number % size - 1 : 0; // -1 because process #0 will get one of them
        for (int proc = 1; proc < size && proc <= rows_number; proc++)
        {            
            int adrow = 0;
            if (remained_rows > 0)
            {
                adrow = 1;
                --remained_rows;
            }
            MPI_Send(global_matr[proc * (rows_per_process + adrow)], 
                (rows_per_process + adrow) * cols_number, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    int remained_rows = rows_number % size;
    int adrow = rank < remained_rows ? 1 : 0;
    std::vector<int> local_vec(rows_per_process * cols_number);
    if (rank == 0) 
    {       
        local_vec = std::vector<int>(global_matr[0], global_matr[rows_per_process + adrow]);
    } 
    else 
    {
        MPI_Status status;
        MPI_Recv(local_vec.data(), (rows_per_process + adrow) * cols_number, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
        
    outf << "Process " << rank << ": what I got: ";
    outf << vtos(local_vec) << std::endl;

    for (int i = 0; i < rows_per_process + adrow; ++i)
    {
        std::vector<int> vector_to_find(local_vec.begin() + i * cols_number, local_vec.begin() + (i + 1) * cols_number);
        max_values[rank + i] = findMax(vector_to_find);
        outf << "Process " << rank << ": I found max at row " << rank + i << ": " << max_values[rank + i] << std::endl;        
    }    
    outf << std::string('-', 30) << std::endl;
    return max_values;
}
