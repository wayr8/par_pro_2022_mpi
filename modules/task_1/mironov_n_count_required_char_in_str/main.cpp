  // Copyright 2022 Mironov Nikita
#include <gtest/gtest.h>
#include <cstring>
#include "../../../modules/task_1/mironov_n_count_required_char_in_str/count_required_char_in_str.h"
#include <gtest-mpi-listener.hpp>

TEST(Count_required_char_in_str, count_an_existing_character_in_string) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    char str[] = "dadsadaababas";
    char requiredChar = 'a';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str, requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str, count_a_missing_character_in_string) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    char str[] = "deaaxabas";
    char requiredChar = 'z';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str, requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str,
    count_character_in_string_containing_only_that_character) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    char str[] = "fffffffffff";
    char requiredChar = 'f';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str, requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str, throw_when_string_is_empty) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    char str[] = "";
    char requiredChar = 'd';

    ASSERT_ANY_THROW(countRequiredCharInStr(str, requiredChar));
}

TEST(Count_required_char_in_str,
    throw_when_create_random_string_with_negative_size) {
    int size = -5;
    char *str = new char[5];
    ASSERT_ANY_THROW(getRandomStr(str, size));
}

TEST(Count_required_char_in_str,
    count_character_in_random_string_and_random_char) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    int size = 10;
    char *str = new char[size];
    char requiredChar;

    getRandomStr(str, size);
    requiredChar = 'a';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str, requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

int main(int argc, char **argv) {
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
