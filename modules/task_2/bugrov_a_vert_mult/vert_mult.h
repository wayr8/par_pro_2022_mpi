#include <mpi.h>
#include <ctime>
#include <random>
void rand_creator(int* matrix_or_vector, int size);
void seq_mult(int* matrix, int* vector, int* result_matrix, int n, int m);
void par_mult(int* matrix, int* vector, int* result_matrix, int n, int m, int process_num);