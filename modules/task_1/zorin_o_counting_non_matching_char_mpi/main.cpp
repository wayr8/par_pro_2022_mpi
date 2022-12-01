// Copyright 2022 Zorin Oleg
#include <gtest/gtest.h>
#include <random>
#include "./counting_non_matching_char.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sequential_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::string global_str = "banana";
        std::string global_compare_str = "apple";
        int count = countNonMatchingCharSequential(global_str, global_compare_str);
        ASSERT_EQ(3, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str;
    std::string global_compare_str;
    if (rank == 0)
    {
        global_str = "banana";
        global_compare_str = "apple";
    }

    int count = countNonMatchingCharParallel(global_str, global_compare_str);

    if (rank == 0)
    {
        ASSERT_EQ(3, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Same_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str;
    std::string global_compare_str;
    size_t string_size = 120;

    if (rank == 0)
    {
        global_str = getRandomString(string_size);
        global_compare_str = getRandomString(string_size);
    }

    int global_count = countNonMatchingCharParallel(global_str, global_compare_str);

    if (rank == 0)
    {
        int reference_count = countNonMatchingCharSequential(global_str, global_compare_str);
        ASSERT_EQ(reference_count, global_count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Diff_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str;
    std::string global_compare_str;
    size_t string_size = 50;
    size_t compare_string_size = 120;

    if (rank == 0)
    {
        global_str = getRandomString(string_size);
        global_compare_str = getRandomString(compare_string_size);
    }

    int global_count = countNonMatchingCharParallel(global_str, global_compare_str);

    if (rank == 0)
    {
        int reference_count = countNonMatchingCharSequential(global_str, global_compare_str);
        ASSERT_EQ(reference_count, global_count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Random_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str;
    std::string global_compare_str;

    if (rank == 0)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<size_t> dist(0, 200);
        size_t string_size = dist(mt);
        size_t compare_string_size = dist(mt);
        global_str = getRandomString(string_size);
        global_compare_str = getRandomString(compare_string_size);
    }

    int global_count = countNonMatchingCharParallel(global_str, global_compare_str);

    if (rank == 0)
    {
        int reference_count = countNonMatchingCharSequential(global_str, global_compare_str);
        ASSERT_EQ(reference_count, global_count);
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
