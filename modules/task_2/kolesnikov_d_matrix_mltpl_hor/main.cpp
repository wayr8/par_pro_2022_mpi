#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

#include "./matrix_column_max.h"





TEST(MATRIX_MLTPL_TEST, multiplication_zero_matrices) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> a;
    vector<int> b;
    vector<int> seq_c;

    if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, 0, 0,
            b, 0, 0);
        ASSERT_EQ(vector<int>(), seq_c);
    }
}



TEST(MATRIX_MLTPL_TEST, multiplication_scalars) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a_size_x = 1;
    int a_size_y = 1;
    int b_size_x = 1;
    int b_size_y = 1;

    vector<int> a{5};
    vector<int> b{5};
    vector<int> seq_c;

    result_matrix =vector<int> {
        1,
    };

    if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, a_size_x, a_size_y,
            b, b_size_x, b_size_y);
    }
    
    vector<int> c = MatrixMtlplSeq(
        a, a_size_x, a_size_y,
        b, b_size_x, b_size_y);

    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
        ASSERT_EQ(result_matrix, c);
    }
}

TEST(MATRIX_MLTPL_TEST, multiplication_vectors) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a_size_x = 5;
    int a_size_y = 1;
    int b_size_x = 1;
    int b_size_y = 5;

    a = vector<int> {
        1, 2, 3
    };
    b = vector<int> {
        1,
        2,
        3,
    };
    vector<int> seq_c;

    result_matrix =vector<int> {
        14,
    };

    if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, a_size_x, a_size_y,
            b, b_size_x, b_size_y);
    }
    
    vector<int> c = MatrixMtlplSeq(
        a, a_size_x, a_size_y,
        b, b_size_x, b_size_y);

    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
        ASSERT_EQ(result_matrix, c);
    }

}
TEST(MATRIX_MLTPL_TEST, multiplication_small_matrices) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a_size_x = 5;
    int a_size_y = 1;
    int b_size_x = 1;
    int b_size_y = 5;

    a = vector<int> {
        1, 2, 3,
        4, 5, 6,
    };
    b = vector<int> {
        1, 4,
        2, 5,
        3, 6,
    };
    vector<int> seq_c;

    result_matrix =vector<int> {
        14, 32,
        32, 77,
    };

    if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, a_size_x, a_size_y,
            b, b_size_x, b_size_y);
    }
    
    vector<int> c = MatrixMtlplSeq(
        a, a_size_x, a_size_y,
        b, b_size_x, b_size_y);

    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
        ASSERT_EQ(result_matrix, c);
    }

}



TEST(MATRIX_MLTPL_TEST, multiplication_big_matrices) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a_size_x = 5;
    int a_size_y = 1;
    int b_size_x = 1;
    int b_size_y = 5;

    a = vector<int> {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        1, 2, 3,
        4, 5, 6,
    };
    b = vector<int> {
        1, 4, 7,
        2, 5, 8,
        3, 6, 8,
    };
    vector<int> seq_c;

    result_matrix =vector<int> {
        14, 32, 47
        32, 77, 116,
        50, 122,185,
        14, 32, 47,
        32, 77, 116,
    };

    if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, a_size_x, a_size_y,
            b, b_size_x, b_size_y);
    }
    
    vector<int> c = MatrixMtlplSeq(
        a, a_size_x, a_size_y,
        b, b_size_x, b_size_y);

    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
        ASSERT_EQ(result_matrix, c);
    }

}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}