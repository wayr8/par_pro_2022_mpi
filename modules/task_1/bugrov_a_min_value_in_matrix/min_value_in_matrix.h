#include <ctime> // for time function
#include <random> // for mt19937

void create_random_values(int* matrix, int size);
void matrix_printer(int* matrix, int n, int m);
void seq_min_value(int* matrix, int size);
void par_min_value(int* matrix, int size, int process_num, int* min_values);