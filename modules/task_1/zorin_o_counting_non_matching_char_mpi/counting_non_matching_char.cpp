// Copyright 2022 Zorin Oleg
#include <mpi.h>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/zorin_o_counting_non_matching_char_mpi/counting_non_matching_char.h"

std::string getRandomString(size_t size)
{
    const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string random_str;
    random_str.reserve(size);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> dist(0, sizeof(alphanum)-1);

    for (size_t i = 0; i < size; i++)
    {
        random_str += alphanum[dist(mt)];
    }

    return random_str;
}
int countNonMatchingCharSequential(const std::string &str,
                                   const std::string &compare_str)
{
    int count = 0;
    for (const char& i : str)
    {
        if (compare_str.find(i) == std::string::npos)
            count++;
    }
    return count;
}
int countNonMatchingCharParallel(const std::string &global_str,
                                 const std::string &global_compare_str)
{
    int proc_size, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    proc_size = global_str.size() > proc_size ? proc_size : global_str.size();
    const int delta =  global_str.size() / proc_size;

    if (proc_rank == 0)
    {
        for (int proc = 1; proc < proc_size; proc++)
        {
            MPI_Send(global_str.data() + proc * delta, delta,
                     MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::string local_str;
    if (proc_rank == 0)
    {
        local_str = std::string(global_str.begin(),
                                global_str.begin() + delta);
    }
    else
    {
        MPI_Status status;
        MPI_Recv((void *) local_str.data(), delta,
                 MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_count = 0;
    int local_count = countNonMatchingCharSequential(local_str, global_compare_str);
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}
