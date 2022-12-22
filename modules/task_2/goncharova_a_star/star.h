// Copyright Anna Goncharova

#ifndef ONEDRIVE___________GITHUB_PAR_PRO_2022_MPI_MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
#define ONEDRIVE___________GITHUB_PAR_PRO_2022_MPI_MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
#include <mpi.h>
#include <iostream>
MPI_Comm createStarComm(const MPI_Comm oldComm);
bool isStarTopology(const MPI_Comm curComm);
#endif  // ONEDRIVE___________GITHUB_PAR_PRO_2022_MPI_MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
