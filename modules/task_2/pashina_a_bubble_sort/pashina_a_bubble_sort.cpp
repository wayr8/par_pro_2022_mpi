// Copyright 2022 Pashina Alina

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include "../../../modules/task_2/pashina_a_bubble_sort/pashina_a_bubble_sort.h" 

double *generateNewArr(int al) {
  std::mt19937 generator;
  double *generated_re = new double[al];
  generator.seed(static_cast<unsigned int>(time(0)));
  int k = 0;
  while (k < al) {
    generated_re[k] = static_cast<int>(generator() % 500);
    k += 1;
  }
  return generated_re;
}

int mconnect(double *first_ar, int len_first_ar, double *sec_ar, int sec_ar_len,
             double *ourfin) {
  int n = 0;
  int s = 0;
  int mycounter_fin = 0;

  for (; n < len_first_ar; n++) {
    while (s < sec_ar_len && (sec_ar[s] < first_ar[n])) {
      ourfin[mycounter_fin++] = sec_ar[s++];
    }
    ourfin[mycounter_fin++] = first_ar[n];
  }
  while (s < sec_ar_len) ourfin[mycounter_fin++] = sec_ar[s++];

  return 0;
}

void Change_Places_MPI(int sizeL, double *arrL, int rankToExp, int recvrank,
                       MPI_Comm comm) {
  int thrank;
  double *part_rem = new double[sizeL];
  double *fin_res_arr = new double[2 * sizeL];
  MPI_Comm_rank(comm, &thrank);
  if (thrank == rankToExp) {
    MPI_Send(arrL, sizeL, MPI_DOUBLE, recvrank, 1, MPI_COMM_WORLD);
    MPI_Recv(arrL, sizeL, MPI_DOUBLE, recvrank, 2, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  } else {
    int beginingStranger = 0;
    MPI_Recv(part_rem, sizeL, MPI_DOUBLE, rankToExp, 1, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    mconnect(arrL, sizeL, part_rem, sizeL, fin_res_arr);
    int beginingMy = sizeL;
    if (rankToExp > thrank) {
      beginingStranger = sizeL;
      beginingMy = 0;
    }
    MPI_Send(&(fin_res_arr[beginingStranger]), sizeL, MPI_DOUBLE, rankToExp, 2,
             MPI_COMM_WORLD);
    for (int i = beginingMy; i < beginingMy + sizeL; i++)
      arrL[i - beginingMy] = fin_res_arr[i];
  }
}

double *typical_bubble_s(double *our_arr_to_sort, int sizearr) {
  for (int xind = 0; xind < sizearr - 1; xind++)
    for (int yind = 0; yind < sizearr - xind - 1; yind++)
      if (our_arr_to_sort[yind] > our_arr_to_sort[yind + 1])
        std::swap(our_arr_to_sort[yind], our_arr_to_sort[yind + 1]);
  return our_arr_to_sort;
}

int start_sort_before_M(double *f_ar, int begining, int finish, double *s_ar) {
  int our_average = (finish + begining) / 2;
  if ((finish - begining) <= 1) return 0;
  start_sort_before_M(f_ar, begining, our_average, s_ar);
  start_sort_before_M(f_ar, our_average, finish, s_ar);
  mconnect(&(f_ar[begining]), our_average - begining, &(f_ar[our_average]),
           finish - our_average, &(s_ar[begining]));

  int p = begining;
  while (p < finish) {
    f_ar[p] = s_ar[p];
    p += 1;
  }

  return 0;
}

int mconnect_sorting(int narr, double *ourin_arr) {
  double *b = new double[narr];
  start_sort_before_M(ourin_arr, 0, narr, b);
  return 0;
}

double *doParallel_BubbleOddEven(int len, double *myarr, int mainstart,
                                 MPI_Comm comm) {
  int thrank;
  double *local_a;
  MPI_Comm_rank(comm, &thrank);
  int thsize;
  MPI_Comm_size(comm, &thsize);

  local_a = (double *)calloc(len / thsize, sizeof(double));
  int index = 0;
  MPI_Scatter(myarr, len / thsize, MPI_DOUBLE, local_a, len / thsize,
              MPI_DOUBLE, mainstart, comm);
  mconnect_sorting(len / thsize, local_a);
  while (index < thsize) {
    if ((thrank + index) % 2 == 0) {
      if (thrank + 1 < thsize) {
        Change_Places_MPI(len / thsize, local_a, thrank, thrank + 1, comm);
      }
    } else if (thrank > 0) {
      Change_Places_MPI(len / thsize, local_a, thrank - 1, thrank, comm);
    }
    index += 1;
  }
  MPI_Gather(local_a, len / thsize, MPI_DOUBLE, myarr, len / thsize, MPI_DOUBLE,
             mainstart, comm);

  return myarr;
}
