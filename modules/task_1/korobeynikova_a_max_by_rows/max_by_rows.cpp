// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_1/korobeynikova_a_max_by_rows/max_by_rows.h"

#include <mpi.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

std::vector<int> taskDistrib(const int proc_num, const int task_num) {
  std::vector<int> task_per_proc(proc_num);
  int remained_rows = task_num % proc_num;
  for (int proc = 0; proc < proc_num; ++proc) {
    task_per_proc.at(proc) = task_num / proc_num + (remained_rows > 0);
    --remained_rows;
  }
  return task_per_proc;
}

int findMax(std::vector<int> vec) {
  const int sz = vec.size();
  int max = 0;

  max = vec.at(0);
  for (int i = 1; i < sz; i++) {
    max = std::max(max, vec.at(i));
  }
  return max;
}

std::string vtos(std::vector<int> v) {
  std::ostringstream oss;
  if (!v.empty()) {
    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(oss, ", "));
    oss << v.back();
  }
  return oss.str();
}

std::vector<int> getParallelOperation(Matrix<int> global_matr) {
  std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                     std::ios_base::app);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows_num = global_matr.GetN();
  const int cols_number = global_matr.GetM();
  std::vector<int> rows_per_process = taskDistrib(size, rows_num);
  std::vector<int> max_values(rows_per_process.at(rank));

  if (rank == 0) {
    outf << "tasks distribution: " << vtos(rows_per_process) << std::endl;
    int rows_to_skip = rows_per_process[0];
    for (int proc = 1; proc < size; ++proc) {      
      MPI_Send(global_matr[rows_to_skip],
               rows_per_process.at(proc) * cols_number, MPI_INT, proc, 0,
               MPI_COMM_WORLD);
      rows_to_skip += rows_per_process.at(proc);
      outf << "Send" << std::endl;
    }
  }

  std::vector<int> local_vec(rows_per_process.at(rank) * cols_number);
  if (rank == 0) {
    local_vec = std::vector<int>(global_matr[0],
                                 global_matr[rows_per_process.at(rank)]);
  } else {
    MPI_Status status;
    MPI_Recv(local_vec.data(), rows_per_process.at(rank) * cols_number, MPI_INT,
             0, 0, MPI_COMM_WORLD, &status);
  }

  outf << "Process " << rank << ": what I got: ";
  outf << vtos(local_vec) << std::endl;
  for (int i = 0; i < rows_per_process.at(rank); ++i) {
    std::vector<int> vector_to_find(local_vec.begin() + i * cols_number,
                                    local_vec.begin() + (i + 1) * cols_number);
    max_values.at(i) = findMax(vector_to_find);
    outf << "Process " << rank << ": I found max at row " << i << ": "
         << max_values.at(i) << std::endl;
  }

  if (rank == 0) {
    MPI_Status status;
    for (int proc = 1; proc < size; ++proc) {
      std::vector<int> rec_data(rows_per_process.at(proc));
      MPI_Recv(rec_data.data(), rec_data.size(),
               MPI_INT, proc, 0,
               MPI_COMM_WORLD, &status);
      outf << "rec_data from p" << proc << ": " << vtos(rec_data) << std::endl;
      max_values.insert(max_values.end(), rec_data.begin(), rec_data.end());
      outf << "max_values: " << vtos(max_values) << std::endl;
    }
  } else {
    outf << rank << " send\n";
    MPI_Send(max_values.data(), max_values.size(), MPI_INT, 0, 0,
             MPI_COMM_WORLD);
  }
  return max_values;
}
