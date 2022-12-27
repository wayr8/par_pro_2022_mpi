// Copyright 2022 Pashina Alina

#pragma once
#include <mpi.h>
double *typical_bubble_s(double *our_arr_to_sort, int sizearr);
double *generateNewArr(int al);
int mconnect_sorting(int narr, double *ourin_arr);
int mconnect(double *first_ar, int len_first_ar, double *sec_ar, int sec_ar_len,
             double *ourfin);
void Change_Places_MPI(int sizeL, double *arrL, int rankToExp, int recvrank,
                       MPI_Comm comm);
int start_sort_before_M(double *f_ar, int begining, int finish, double *s_ar);
double *doParallel_BubbleOddEven(int len, double *myarr, int mainstart,
                                 MPI_Comm comm);