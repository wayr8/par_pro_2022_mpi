  // Copyright 2022 Mironov Nikita
#ifndef MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR
#define MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR

#include <mpi.h>
#include <random>
#include <cstring>
#include <iostream>

void getRandomStr(char *str, int strSize);

void getRandomRequiredChar(char *requiredChar);

int countRequiredCharInProcStr(char *str, char requiredChar);

int countRequiredCharInStr(char *str, char requiredChar);

#endif  // _MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR
