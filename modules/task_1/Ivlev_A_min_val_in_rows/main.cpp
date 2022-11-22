// Copyright 2022 Ivlev A
#include <gtest/gtest.h>
#include <vector>
#include "./min_val_in_rows.h"
#include <gtest-mpi-listener.hpp>
//#include <iostream>


TEST(test_MPI, test_min_0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_matrix;

    int m = 5;
    int n = 4;

    if (rank == 0) {
        global_matrix = getRandomMatrix(m, n);
    }
    
    int* global_min = getParallelMin(global_matrix, m, n);

    if(rank == 0){   
        int* global_min_local = getMatrixMinbyRow(global_matrix, m, n);

        for(size_t i = 0; i < m; i++) {
            ASSERT_EQ(global_min_local[i], global_min[i]);
        }
        
        delete [] global_matrix;
        delete [] global_min;
        delete [] global_min_local;
    }
}

TEST(test_MPI, test_min_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_matrix;

    int m = 4;
    int n = 6;

    if (rank == 0) {
        global_matrix = getRandomMatrix(m, n);
    }
    
    int* global_min = getParallelMin(global_matrix, m, n);

    if(rank == 0){   
        int* global_min_local = getMatrixMinbyRow(global_matrix, m, n);

        for(size_t i = 0; i < m; i++) {
            ASSERT_EQ(global_min_local[i], global_min[i]);
        }
        
        delete [] global_matrix;
        delete [] global_min;
        delete [] global_min_local;
    }
}

TEST(test_MPI, test_min_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_matrix;

    int m = 4;
    int n = 4;

    if (rank == 0) {
        global_matrix = getRandomMatrix(m, n);
    }
    
    int* global_min = getParallelMin(global_matrix, m, n);

    if(rank == 0){   
        int* global_min_local = getMatrixMinbyRow(global_matrix, m, n);

        for(size_t i = 0; i < m; i++) {
            ASSERT_EQ(global_min_local[i], global_min[i]);
        }
        
        delete [] global_matrix;
        delete [] global_min;
        delete [] global_min_local;
    }
}

TEST(test_MPI, test_min_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_matrix;

    int m = 11;
    int n = 12;

    if (rank == 0) {
        global_matrix = getRandomMatrix(m, n);
    }
    
    int* global_min = getParallelMin(global_matrix, m, n);

    if(rank == 0){   
        int* global_min_local = getMatrixMinbyRow(global_matrix, m, n);

        for(size_t i = 0; i < m; i++) {
            ASSERT_EQ(global_min_local[i], global_min[i]);
        }
        
        delete [] global_matrix;
        delete [] global_min;
        delete [] global_min_local;
    }
}

TEST(test_MPI, test_min_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_matrix;

    int m = 2;
    int n = 20;

    if (rank == 0) {
        global_matrix = getRandomMatrix(m, n);
    }
    
    int* global_min = getParallelMin(global_matrix, m, n);

    if(rank == 0){   
        int* global_min_local = getMatrixMinbyRow(global_matrix, m, n);

        for(size_t i = 0; i < m; i++) {
            ASSERT_EQ(global_min_local[i], global_min[i]);
        }
        
        delete [] global_matrix;
        delete [] global_min;
        delete [] global_min_local;
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
