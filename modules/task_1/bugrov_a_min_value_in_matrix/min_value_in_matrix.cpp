  
#include <mpi.h>
#include "min_value_in_matrix.h"

// size = n * m
void create_random_values(int* matrix, int size)
{
    std::mt19937 engine(time(0));
    for (int i = 0; i < size; i++) {
        matrix[i] = engine() % 100000;
    }
}

void matrix_printer(int* matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << "\n";
    }
}

int seq_min_value(int* matrix, int size)
{
    int min_value = matrix[0][0]
    for (int i = 1; i < size; i++) {
        if (matrix[i] < min_value) {
            min_value = matrix[i];
        }
    }
    return min_value;
}

int par_min_value(int* matrix, int size, int process_num)
{
    int rank = 0;
    int part = size / process_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* min_values = new int[process_num];
    if (rank == 0) {
        MPI_Scatter(matrix, part, MPI_INT, 0, MPI_INT, MPI_COMM_WORLD);
        int remainder = size - part * process_num;
        if (remainder) {
            int* zero_part = matrix + part * process_num;
            min_values[0] = zero_part[0];
            for (int i = 1; i < remainder; i++) {
                if (min_values[0] > zero_part[i]) {
                    min_values[0] = zero_part[i];
                }
            }
        }
    }
    else {
        int* recv_buf;
        recv_buf = new int[part];
        MPI_Scatter(matrix, part, MPI_INT, 0, MPI_INT, MPI_COMM_WORLD);
        min_values[rank] = recv_buf[0];
        for (int i = 1; i < part; i++) {
            if (min_values[0] > recv_buf[i]) {
                min_values[0] = recv_buf[i];
            }
        }
    }
}
