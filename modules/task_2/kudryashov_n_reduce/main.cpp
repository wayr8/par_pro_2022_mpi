  // Copyright 2022 Kudryashov Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <random>
#include <ctime>
#include "./kudryashov_n_reduce.h"
#include <gtest-mpi-listener.hpp>

TEST(Reduce, test_single_int_sum) {
    int proc_num, rank, root;
    int reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_int_prod) {
    int proc_num, rank, root;
    int reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_int_max) {
    int proc_num, rank, root;
    int reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_int_min) {
    int proc_num, rank, root;
    int reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1
            time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_int_sum) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> randVec = generateRandomVector<int>(15);
    int count = 15 / proc_num;
    std::vector<int> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_INT, vec.data(), count, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_int_prod) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> randVec = generateRandomVector<int>(15);
    int count = 15 / proc_num;
    std::vector<int> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_INT, vec.data(), count, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_int_max) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> randVec = generateRandomVector<int>(15);
    int count = 15 / proc_num;
    std::vector<int> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_INT, vec.data(), count, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_int_min) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> randVec = generateRandomVector<int>(15);
    int count = 15 / proc_num;
    std::vector<int> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_INT, vec.data(), count, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_double_sum) {
    int proc_num, rank, root;
    double reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_double_prod) {
    int proc_num, rank, root;
    double reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_double_max) {
    int proc_num, rank, root;
    double reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_double_min) {
    int proc_num, rank, root;
    double reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_double_sum) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> randVec = generateRandomVector<double>(15);
    int count = 15 / proc_num;
    std::vector<double> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_DOUBLE, vec.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_double_prod) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> randVec = generateRandomVector<double>(15);
    int count = 15 / proc_num;
    std::vector<double> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_DOUBLE, vec.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_double_max) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> randVec = generateRandomVector<double>(15);
    int count = 15 / proc_num;
    std::vector<double> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_DOUBLE, vec.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_double_min) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> randVec = generateRandomVector<double>(15);
    int count = 15 / proc_num;
    std::vector<double> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_DOUBLE, vec.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_float_sum) {
    int proc_num, rank, root;
    float reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_float_prod) {
    int proc_num, rank, root;
    float reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_float_max) {
    int proc_num, rank, root;
    float reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_single_float_min) {
    int proc_num, rank, root;
    float reducedData = 0, myReducedData = 0;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float data = 10 * (rank + 1);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(&data, &reducedData, 1, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(&data, &myReducedData, 1, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_float_sum) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<float> randVec = generateRandomVector<float>(15);
    int count = 15 / proc_num;
    std::vector<float> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_FLOAT, vec.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_float_prod) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<float> randVec = generateRandomVector<float>(15);
    int count = 15 / proc_num;
    std::vector<float> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_FLOAT, vec.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_float_max) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<float> randVec = generateRandomVector<float>(15);
    int count = 15 / proc_num;
    std::vector<float> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_FLOAT, vec.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
    }
}

TEST(Reduce, test_vector_float_min) {
    int proc_num, rank, root;
    double time1, time2;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<float> randVec = generateRandomVector<float>(15);
    int count = 15 / proc_num;
    std::vector<float> reducedData(count), myReducedData(count), vec(count);

    if (rank == 0) {
        std::mt19937 rnd;
        rnd.seed(std::time(nullptr));
        root = rnd() % proc_num;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(randVec.data(), count, MPI_FLOAT, vec.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    MPI_Reduce(vec.data(), reducedData.data(), count, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    reduce(vec.data(), myReducedData.data(), count, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - time2;
        printf("MPI Reduce: %lf\n", time1);
        printf("My Reduce: %lf\n", time2);
        ASSERT_EQ(reducedData, myReducedData);
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
