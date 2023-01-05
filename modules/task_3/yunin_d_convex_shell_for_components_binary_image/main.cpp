// Copyright 2023 Yunin Daniil
#include <gtest/gtest.h>
#include <vector>
#include "./convex_shell_for_components_binary_image.h"
#include <gtest-mpi-listener.hpp>

// итак, что мы имеем на входе - бинарное изображение (массив, значения которого 0 и 1, с несколькими компонентами связности)
// выход - массив точек, которые образуют выпуклую оболочку этих компонент

// по идеи, получается, что я должен написать параллельную реализацию алгоритма для построения выпуклой оболочки и последовательную и на этом всё.


TEST(Convex_Shell_Binary_Image, Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec(7*7);

    if (rank == 0) {
        CreateImage1(global_vec);
        PrintImage(global_vec, 7, 7);
    }

    if (rank == 0) {
        JarvisMarch(global_vec, 7, 7);
        ASSERT_EQ(1, 1);
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
