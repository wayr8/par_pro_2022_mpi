// Copyright 2022 Yunin Daniil
#include "../../../modules/task_2/yunin_d_gauss_ribbon_horizontal_scheme/gauss_ribbon_horizontal_scheme.h"
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>

using std::vector;
using std::mt19937;
using std::random_device;

// Метод Гаусса используется для решения систем алгебраических уравнений
// Матрицы являются плотными - большинство элементов отличны от нуля
// Если СЛУ - невырожденная (определить матрицы коэффициентов системы отличен от нуля) - решение с определённой точностью
// Подразумевается, что решаем квадратные матрицы
//Решение системы линейных уравнений существует только в случае, когда матрица системы линейных уравнений невырожденная (определитель отличен от нуля)

// Help functions start
void UpdateRandNumbers(mt19937 *gen) {
    random_device rd;
    (*gen).seed(rd());
}
// Help functions end

// Сonsequent Gauss Start

// 1. Метод создания случайной квадратной матрицы
// 2. Решение системы
// 2.1. Прямой ход алгоритма
// 2.2. Обратный ход алгоритма
// 3. Вывод результата
// 4. Посчитать определитель?

void CreateMatrix(vector<double> &matrix, int size) {
    // чтобы сразу было всё хорошо (невырожденная матрица типа)
    /*
    1 2 3 4
    2 1 2 3
    3 2 1 2
    4 3 2 1
    */
    int number = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i*size+j] = number;
            (j < i) ? number-- : number++;
        }
        number = i+2;
    }
    
}

void CreateVector(vector<double> &vec, int size) {
    // чтобы сразу было всё хорошо
    for (int i = 0; i < size; i++) {
        vec[i] = i*i*i;
    }
    

}

void CreateMatrixRandom(vector<double> &matrix, int size, mt19937 *gen) {
    // дело случая
    for (int i = 0; i < size * size; i++) {
        matrix[i] = (*gen)() % size; 
    }
    
}

void CreateVectorRandom(vector<double> &vec, int size, mt19937 *gen) {
    // дело случая
    for (int i = 0; i < size; i++) {    
        vec[i] = (*gen)() % size + 1;
    }
    
}

// add this function
void InitHelpingVector(vector<int> &vec, int size) {
    for (int i = 0; i < size; i++) {
        vec[i] = -1; // указывает, что данная строка не была ещё выбрана в качестве ведущей
    }   
}

// add this function in header file
void SubtractCurrentRowMatrix(vector<double> &matr, int size, int num_row, int num_column, double main_elem, double coef) {
    for (int j = num_column; j < size; j++) {
        // std::cout << std::endl << "Вычитаю элемент - (" << num_row << ',' << j << ')' << std::endl; 
        matr[num_row*size+j] = matr[num_row*size+j] - coef*matr[num_column*size+j]; //можно использовать num_column, так как в данном случае = номеру главной строки
    }
}

// add this function in header file
void SubtractCurrentRowVector(vector<double> &vec, int size, int num_iter, double main_elem, double coef) {
    vec[num_iter] = vec[num_iter] - coef*main_elem;
}

// add this function in header file
double CalculateCoef(double main_elem, double reduced_elem) {
    double coef = reduced_elem/main_elem;
    return coef;
}


vector<double> GaussConsequent(int matrix_size) {
    vector<double> matrix(matrix_size * matrix_size); // матрица коэффициентов (одномерный массив, где элементы хранятся построчно) индекс массива - i*size-matrix+j
    vector<double> right_vector(matrix_size); // правая части (после знака =) СЛАУ
    vector<double> results(matrix_size); // вектор с ответами (значения вычесленных x)
    // helping vectors
    vector<int> sequence_numbers_rows(matrix_size); // номер строки, которая была выбрана на i-ом шаге
    vector<int> sequence_numbers_iterations(matrix_size);// номер итерации на котором i-ая строка была выбрана
    // helping vectors
    const int size_matrix = matrix_size; // на всякий случай сохраню
    mt19937 gen;
    UpdateRandNumbers(&gen);
    // инициализирую матрицу и вектор значениями
    CreateMatrix(matrix, size_matrix);
    CreateVector(right_vector, size_matrix);
    InitHelpingVector(sequence_numbers_iterations, size_matrix);
    // прямой ход
    // for (int i = 0; i < size_matrix; i++) {
    //         for (int j = 0; j < size_matrix; j++) {
    //             std::cout << matrix[i*size_matrix+j] << ' ';
    //         }
    //         std::cout << std::endl;
    //     }
    // std::cout << std::endl;
    // for (int i = 0; i < size_matrix; i++) {
    //     std::cout << right_vector[i] << ' ';
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;
    for (int i = 0; i < size_matrix; i++) {
        double current_matrix_elem = matrix[i*size_matrix+i];
        double current_vector_elem = right_vector[i];
        double coef = 0;
        for (int j = i+1; j < size_matrix; j++) {
            coef = CalculateCoef(current_matrix_elem, matrix[j*size_matrix+i]);
            // std::cout << "coef = " << coef;
            // std::cout << std::endl;
            SubtractCurrentRowMatrix(matrix, size_matrix, j, i, current_matrix_elem, coef);
            SubtractCurrentRowVector(right_vector, size_matrix, j, current_vector_elem, coef);
            // std::cout << std::endl;
            // for (int k = 0; k < size_matrix; k++) {
            //     for (int l = 0; l < size_matrix; l++) {
            //         std::cout << matrix[k*size_matrix+l] << ' ';
            //     }
            //     std::cout << std::endl;
            // }
            // std::cout << std::endl;
            // for (int i = 0; i < size_matrix; i++) {
            //     std::cout << right_vector[i] << ' ';
            // }
            // std::cout << std::endl;
        
        }
    }
    // std::cout << std::endl;
    // std::cout << std::endl;
    // // прямой ход
    // std::cout << std::endl;
    // for (int k = 0; k < size_matrix; k++) {
    //     for (int l = 0; l < size_matrix; l++) {
    //         std::cout << matrix[k*size_matrix+l] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    // for (int i = 0; i < size_matrix; i++) {
    //     std::cout << right_vector[i] << ' ';
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;
    // обратный ход
    for (int i = size_matrix-1; i >= 0; i--) {
        // пересчитываем матрицу
        for (int j = size_matrix-1; j > i; j--) {
            // std::cout << "элементы строки матрицы " << matrix[i*size_matrix+j] << std::endl;
            right_vector[i] = right_vector[i] - results[j]*matrix[i*size_matrix+j];
            matrix[i*size_matrix+j] = 0;
        }
        results[i] = right_vector[i] / matrix[i*size_matrix+i];
        // std::cout << "элемент матрицы " << matrix[i*size_matrix+i] << std::endl;
    }
    // for (int i = 0; i < size_matrix; i++) {
    //     std::cout << results[i] << ' ';
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;
    // return right_vector; DEBUG
    // return matrix;
    return results; // True code
} 


// Сonsequent Gauss End

void PrintMatrix(vector<double> matr, int size_matr) {
    std::cout << "Матрица\n";
    for (int k = 0; k < size_matr; k++) {
        for (int l = 0; l < size_matr; l++) {
            std::cout << matr[k*size_matr+l] << ' ';
        }
        std::cout << std::endl;
    }
}

void PrintVector(vector<double> vec, int vec_size) {
    std::cout << "Правая часть матрицы\n";
    for (int i = 0; i < vec_size; i++) {
        std::cout << vec[i] << ' ';
    }
    std::cout << std::endl;
}

void DistributionData(vector<double> &matr, vector<double> &r_vec, vector<double> &proc_matr, 
    vector<double> &proc_r_vec, int size_matr, int row_num, int size_ranks, int proc_rank) {
    vector<int> send_num(size_ranks); // количество элементов, посланных процессу
    vector<int> send_indexes(size_ranks); // адрес начала посланных элементов
    int rest_rows = size_matr;
    send_num[0] = row_num*size_matr;
    send_indexes[0] = 0;
    // std::cout << row_num << std::endl;
    for (int i = 1; i < size_ranks; i++) {
        rest_rows -= row_num;
        row_num = rest_rows/(proc_rank - i);
        send_num[i] = row_num*size_matr;
        send_indexes[i] = send_indexes[i-1] + send_num[i-1];
    }
    MPI_Scatterv(&matr[0], &send_num[0], &send_indexes[0], MPI_DOUBLE, &proc_matr[0], send_num[proc_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
}

void TestDistribution(vector<double> &pMatrix, vector<double> &pVector, vector<double> &pProcRows,
    vector<double> &pProcVector, int Size, int RowNum, int ProcNum, int ProcRank) {
    if (ProcRank == 0) {
        printf("Initial Matrix: \n");
        PrintMatrix(pMatrix, Size);
        printf("Initial Vector: \n");
        PrintVector(pVector, Size);
    }
    for (int i=0; i<ProcNum; i++) {
        if (ProcRank == i) {
            printf("\nProcRank = %d \n", ProcRank);
            printf(" Matrix Stripe:\n");
            PrintMatrix(pProcRows, RowNum);
            printf(" Vector: \n");
        // PrintVector(pProcVector, RowNum);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

// Parallel Gauss Start

vector<double> GaussParallel(vector<double> &matr, vector<double> &right_part, int size_matr) {
    int proc_rank, proc_size;
    vector<double> global_result;
    
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    vector<int> proc_row_ind(proc_size), proc_row_num(proc_size);
    int num_rows = size_matr / proc_size;
    int remaining_rows = size_matr % proc_size;

    // чтобы распределить строки между процессами равномерно,
    // положим в те процессы, чей ранк меньше остатка на одну строку больше
    int proc_num_rows;
    if (proc_rank < remaining_rows) {
        proc_num_rows = num_rows + 1;
    }
    else {
        proc_num_rows = num_rows;
    }
    // отлаживаем число строк в процессе
    // std::cout << "Число строк " << proc_rank << proc_num_rows << std::endl;
    
    if (proc_rank == 0) {
    //     matr.resize(size_matr*size_matr);
    //     right_part.resize(size_matr);
    //     global_result.resize(size_matr);
    //     CreateMatrix(matr, size_matr);
    //     CreateVector(right_part, size_matr);
        PrintMatrix(matr, size_matr); // для отладки
        PrintVector(right_part, size_matr); // для отладки
    }
    // MPI_Barrier(MPI_COMM_WORLD);   
    

    vector<double> local_matr(proc_num_rows * size_matr);
    vector<double> local_right_vector(proc_num_rows);
    for (int i = 0; i < proc_size; i++) {
        if (proc_rank == i) {
            std::cout << "номер процесса = " << proc_rank << std::endl;
            std::cout << "размер данной матрицы = " << local_matr.size() << std::endl;  
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    vector<int> num_send_elems(proc_size), ind_send_elems(proc_size);

    ind_send_elems[0] = 0;

    // указываем сколько элементов будет послано в каждый процесс
    // и начала этих массивов элементов в массиве матрицы
    for (int i = 0; i < proc_size; i++) {
        if (i < (size_matr % proc_size)) {
            num_send_elems[i] = proc_num_rows * size_matr; 
        }
        else {
            num_send_elems[i] = num_rows * size_matr;
        }
        if (i != 0) {
            std::cout << "номер процесса = " << proc_rank << 
                " индекс предыдущего " << ind_send_elems[i-1] << " количество элементов " << num_send_elems[i-1] << std::endl;
            ind_send_elems[i] = (ind_send_elems[i-1] + num_send_elems[i-1]);
        }
    }
    // for (int i = 0; i < proc_size; i++) {
    //     if (proc_rank == i) {
    //         std::cout << "номер процесса = " << proc_rank << std::endl;
    //         std::cout << "размер данных = " << num_send_elems[i] << std::endl;  
    //         std::cout << "индексы = " << ind_send_elems[i] << std::endl;  
    //     }
    //     MPI_Barrier(MPI_COMM_WORLD);
    // }
    // std::cout << "Процесс - " << proc_rank << std::endl;
    // MPI_Barrier(MPI_COMM_WORLD);   
    // PrintMatrix(matr, size_matr);
    MPI_Scatterv(matr.data(), num_send_elems.data(), ind_send_elems.data(),
        MPI_DOUBLE, &local_matr[0], proc_num_rows * size_matr, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    proc_row_ind[0] = 0; // номер первой строки в нулевом процессе
    proc_row_num[0] = proc_num_rows; // сколько строк на нулевом процессе
    for (int i = 0; i < proc_size; i++) {
        if (i < (size_matr % proc_size)) {
            proc_row_num[i] = proc_num_rows; 
        }
        else {
            proc_row_num[i] = num_rows;
        }
        if (i != 0) {
            std::cout << "номер процесса = " << proc_rank << 
                " индекс предыдущего " << proc_row_ind[i-1] << " количество элементов " << proc_row_num[i-1] << std::endl;
            proc_row_ind[i] = (proc_row_ind[i-1] + proc_row_num[i-1]);
        }
    }
    
    MPI_Scatterv(right_part.data(), proc_row_num.data(), proc_row_ind.data(),
        MPI_DOUBLE, &local_right_vector[0], proc_num_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i=0; i<proc_size; i++) {
        if (proc_rank == i) {
            printf("\nProcRank = %d \n", proc_rank);
            printf(" Matrix Stripe:\n");
            for (int k = 0; k < proc_num_rows; k++) {
                for (int l = 0; l < size_matr; l++) {
                    std::cout << local_matr[k*size_matr+l] << ' ';
                }
                std::cout << std::endl;
            }
            printf(" Vector: \n");
            for (int j = 0; j < proc_num_rows; j++) {
                std::cout << local_right_vector[j] << ' ';
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // прямой ход
    // пока не понятно
    vector<double> main_row(size_matr+1); // буфер для рассылки
    double main_elem , coef;
    if (proc_rank == 0) {
        for (int i = 0; i < size_matr+1; i++) {
          main_row[i] = local_matr[i];
        }
        main_row[size_matr] = local_right_vector[0];
        MPI_Bcast(&main_row[0], size_matr+1, MPI_DOUBLE, proc_rank, MPI_COMM_WORLD);
    }

    for (int i=0; i<proc_size; i++) {
        if (proc_rank == i) {
            printf("\nProcRank = %d \n", proc_rank);
            printf(" Vector_1: \n");
            for (int j = 0; j < size_matr+1; j++) {
                std::cout << main_row[i] << ' ';
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    // for (int i = 0; i < local_matr[proc_rank] / size_matr; i++) {
    //     // берём первый ненулёвой, получается
    //     for (int j = 0; j < size_matr; j++) {
    //         main_row[j] = local_matr[i*size_matr+j];
    //     }
    //     main_row[size_matr] = local_right_vector[i];
    //     // std::cout << "proc - " << proc_rank << "\n";
    //     // for (int j = 0; j < size_matr+1; j++) {
    //     //     std::cout << main_row[j] << ' ';
    //     // }
    //     // std::cout << std::endl;
    //     MPI_Bcast(&main_row[0], size_matr+1, MPI_DOUBLE, proc_rank, MPI_COMM_WORLD);
        
        // for (int k=0; k<proc_size; k++) {
        // if (proc_rank == k) {
        //     printf("\nProcRank = %d \n", proc_rank);
        //     printf(" Vector_Main: \n");
        //     for (int j = 0; j < size_matr+1; j++) {
        //         std::cout << main_row[j] << ' ';
        //     }
        //     std::cout << std::endl;
        // }
        // MPI_Barrier(MPI_COMM_WORLD);
    
    
    
    
    

    // обратный ход
    // сбор результатов

    return global_result;
}

vector<double> GaussParallels(int size_matr) {
    int size_ranks, current_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    // int num_rows = size_matr / size_ranks;
    
    
    
    
    int part = size_matr / size_ranks; // part - число строк в процессе
    // выделяем массивы для частей матрицы и вектора
    vector<double> proc_matr(part*size_matr);
    vector<double> proc_right_part(part);
    vector<double> proc_result(part);
    vector<double> matr;
    vector<double> right_part;
    vector<double> result;

    MPI_Bcast(&size_matr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int rest_rows = size_matr;
    for (int i=0; i<size_matr; i++)
        rest_rows = rest_rows-rest_rows/(size_ranks-i);
    part = rest_rows/(size_ranks-current_rank);

    // готовим данные в нулевом процессе
    if (current_rank == 0) {
        part = size_matr - part * (size_ranks-1);
        // std::cout << part << std::endl;
    } else {
        // std::cout << part << std::endl;
    }
    if (current_rank == 0) {
        matr.resize(size_matr*size_matr);
        right_part.resize(size_matr);
        result.resize(size_matr);
        CreateMatrix(matr, size_matr);
        CreateVector(right_part, size_matr);
        PrintMatrix(matr, size_matr);
        PrintVector(right_part, size_matr);
    }
    
    return matr;
    // у нас уже готовы данные, получается, нужно начинать вычислять
    // я хочу способом, когда часть выделяется процессам, а остатки нулевому
}
// Parallel Gauss End