#ifndef REDUCE
#define REDUCE
#include<stdafx.h>
#include<mpi.h>
void DoWork(MPI_Op op);
void Reduce(int* sendbuf, int* recvbuf, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm);
#endif
