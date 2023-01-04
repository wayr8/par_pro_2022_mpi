// Copyright 2022 Yarakhtin Anton
#ifndef MODULES_TASK_2_YARAKHTIN_ANTON_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_
#define MODULES_TASK_2_YARAKHTIN_ANTON_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_

#include <vector>
#include <mpi.h>

struct Vector2
{
	int x;
	int y;
};

struct TorusElement
{
	Vector2 pos;
	Vector2 size;
	MPI_Comm comm_x;
	MPI_Comm comm_y;
};

Vector2 getCoordinates(int rank, const Vector2& size);
int getRank(const Vector2& coord, const Vector2& size);
TorusElement torusCreate(const Vector2& size);
void torusSend(void* buf, int count, MPI_Datatype datatype, int src, int dst, int tag, const TorusElement& tor);

#endif  // MODULES_TASK_2_YARAKHTIN_ANTON_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_
