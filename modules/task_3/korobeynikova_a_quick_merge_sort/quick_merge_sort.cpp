// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_3/korobeynikova_a_quick_merge_sort/quick_merge_sort.h"

#include <mpi.h>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

using IntVector = std::vector<int>;
using IntVectorPtr = std::unique_ptr<IntVector>;

static std::string vtos(IntVector v) {
  std::ostringstream oss;
  if (!v.empty()) {
    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(oss, ", "));
    oss << v.back();
  }
  return oss.str();
}

IntVector taskDistrib(const int proc_num, const int task_num) {
  IntVector task_per_proc(proc_num);
  int remained_rows = task_num % proc_num;
  for (int proc = 0; proc < proc_num; ++proc) {
    task_per_proc.at(proc) = task_num / proc_num + (remained_rows > 0);
    --remained_rows;
  }
  return task_per_proc;
}

IntVector parallelSort(const IntVector &global_vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int elems_num = global_vec.size();
  if (elems_num == 0) {
    return IntVector();
  } else if (elems_num == 1) {
    return IntVector({global_vec.at(0)});
  }

  IntVector elems_per_process = taskDistrib(size, elems_num);
  IntVector sorted_vec(elems_per_process[rank]);

  std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                     std::ios_base::app);
  if (rank == 0) {
    outf << "We sort this vector: " << vtos(global_vec) << std::endl;
    outf << "tasks distribution: " << vtos(elems_per_process) << std::endl;
    int elems_to_skip = elems_per_process.at(0);
    for (int proc = 1; proc < size; ++proc) {
      outf << "Process 0: I send to Process " << proc << " this: "
           << vtos(IntVector(global_vec.begin() + elems_to_skip,
                                    global_vec.begin() + elems_to_skip +
                                        elems_per_process.at(proc)))
           << std::endl;
      MPI_Send(const_cast<int *>(&global_vec.at(elems_to_skip)),
               elems_per_process.at(proc), MPI_INT, proc, 0, MPI_COMM_WORLD);
      elems_to_skip += elems_per_process.at(proc);
    }
  }

  IntVector local_vec(elems_per_process.at(rank));
  if (rank == 0) {
    local_vec = IntVector(
        global_vec.begin(), global_vec.begin() + elems_per_process.at(rank));
  } else {
    MPI_Status status;
    MPI_Recv(local_vec.data(), elems_per_process.at(rank), MPI_INT, 0, 0,
             MPI_COMM_WORLD, &status);
  }
  outf << "Process " << rank << ": I sort this: " << vtos(local_vec)
       << std::endl;

  for (int i = 0; i < elems_per_process.at(rank); ++i) {
    std::sort(local_vec.begin(), local_vec.end());
  }
  outf << "Process " << rank << ": I sorted and get this: " << vtos(local_vec)
       << std::endl;

  if (rank == 0) {
    MPI_Status status;
    std::vector<IntVectorPtr> ptr_vec(elems_per_process.size());
    ptr_vec.at(0) = std::make_unique<IntVector>(local_vec);
    outf << "ptr_vec[0] consist this: " << vtos(*ptr_vec.at(0)) << std::endl;
    
    for (int proc = 1; proc < size; ++proc) {
      ptr_vec.at(proc) =
          std::make_unique<IntVector>(elems_per_process.at(proc));
      MPI_Recv(ptr_vec.at(proc)->data(), ptr_vec.at(proc)->size(), MPI_INT, proc,
               0,
               MPI_COMM_WORLD, &status);
      outf << "Process " << rank << ": I got from Process " << proc
           << " this : " << vtos(*ptr_vec.at(proc)) << std::endl;
    }

    for (int i = 0; i < ptr_vec.size(); ++i)
    {
      outf << "ptr_vec[" << i << "] = " << vtos(*ptr_vec[i]) << std::endl;
    }

    while (ptr_vec.size() != 1) {
      int index = 0;
      int resize = ptr_vec.size() / 2;
      for (int i = 1; i < ptr_vec.size() - ptr_vec.size() % 2;
           i += 2, ++index) {
        IntVector &f_v = *ptr_vec.at(i - 1);
        IntVector &s_v = *ptr_vec.at(i);

        outf << "Process 0: I'm going to merge ptr_vec[" << i - 1
             << "]: " << vtos(f_v) << " and ptr_vec[" << i << "]: " << vtos(s_v)
             << "\nAnd put it instead of this: " << vtos(*ptr_vec.at(index))
             << std::endl;

        IntVector temp(f_v.size() + s_v.size());
        std::merge(f_v.begin(), f_v.end(), s_v.begin(), s_v.end(),
                   temp.begin());
        *ptr_vec.at(index) = temp;
        outf << "ptr_vec[" << index
             << "] consist this: " << vtos(*ptr_vec.at(0)) << std::endl;
      }
      if (ptr_vec.size() % 2 != 0) {
        //++resize;
        IntVector &f_v = *ptr_vec.at(index - 1);
        IntVector &s_v = *ptr_vec.at(size - 1);
        IntVector temp(f_v.size() + s_v.size());
        std::merge(f_v.begin(), f_v.end(), s_v.begin(), s_v.end(),
                   temp.begin());
        *ptr_vec.at(index - 1) = temp;
      }
      ptr_vec.resize(resize);
      outf << "new ptr_vec size = " << ptr_vec.size() << std::endl;
      outf << "ptr_vec[" << 0 << "] consist this: " << vtos(*ptr_vec.at(0))
           << std::endl;
    }
    return *ptr_vec.at(0);
  } else {
    MPI_Send(local_vec.data(), local_vec.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return local_vec;
}
