#include "vert_mult.h"
void rand_creator(int* matrix_or_vector, int size) 
{
    std::mt19937 engine(time(0));
    for (int i = 0; i < size; i++) {
        matrix[i] = engine() % 173;
    }
}
void seq_mult(int* matrix, int* vector, int* result_matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result_matrix[i] += matrix[i * m + j] * vector[j];
        }
    }
}
void par_mult(int* matrix, int* vector, int* result_matrix, int n, int m, int process_num)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int part = m / process_num;
    int reminder = m % process_num;
    if (rank == 0) {
        // divide values
        MPI_Bcast(matrix, n * m, MPI_INT, rank, MPI_COMM_WORLD);
        for()
        /// mpi_scatter
        /// mpi_gather
        /// summing values
    }
    else {
        /// mpi_scatter
        /// mpi_gather
    }
}