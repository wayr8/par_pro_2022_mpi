// Sasa_2Lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"Reduce.h"
extern int ProcRank;
extern int ProcSize;
int main(int argc,char** argv)
{
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    DoWork(MPI_MAX);
    MPI_Finalize();
}

