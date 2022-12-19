// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_3/korobeynikova_a_quick_merge_sort/quick_merge_sort.h"

#include <mpi.h>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <queue>

using IntVector = std::vector<int>;
using IntVectorPtr = std::shared_ptr<IntVector>;

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
  if (elems_per_process.at(rank) == 0)
  {
    return IntVector{};
  }

  IntVector sorted_vec(elems_per_process[rank]);

  std::ofstream outf("C:/Users/User/Desktop/process_input.txt",
                     std::ios_base::app);
  if (rank == 0) {
    outf << "We sort this vector: " << vtos(global_vec) << std::endl;
    outf << "tasks distribution: " << vtos(elems_per_process) << std::endl;
    int elems_to_skip = elems_per_process.at(0);
    for (int proc = 1; proc < size; ++proc) {
      if (elems_per_process.at(proc) != 0) {
        outf << "Process 0: I send to Process " << proc << " this: "
             << vtos(IntVector(global_vec.begin() + elems_to_skip,
                               global_vec.begin() + elems_to_skip +
                                   elems_per_process.at(proc)))
             << std::endl;

        MPI_Send(const_cast<int *>(&global_vec.at(elems_to_skip)),
                 elems_per_process.at(proc), MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
      elems_to_skip += elems_per_process.at(proc);
    }
  }

  IntVector local_vec(elems_per_process.at(rank));
  if (rank == 0) {
    local_vec = IntVector(
        global_vec.begin(), global_vec.begin() + elems_per_process.at(rank));
  } else {
    MPI_Status status;
    outf << rank << " wait for receive\n";
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
    std::queue<IntVectorPtr> ptr_queue;
    ptr_queue.push(std::make_shared<IntVector>(local_vec));
    outf << "ptr_queue[0] consist this: " << vtos(*ptr_queue.back()) << std::endl;
    
    for (int proc = 1; proc < size; ++proc) {
      if (elems_per_process.at(proc) != 0) {
        ptr_queue.push(std::make_shared<IntVector>(elems_per_process.at(proc)));
        MPI_Recv(ptr_queue.back()->data(), ptr_queue.back()->size(), MPI_INT,
                 proc, 0, MPI_COMM_WORLD, &status);
        outf << "Process " << rank << ": I got from Process " << proc
             << " this : " << vtos(*ptr_queue.back()) << std::endl;
      }
    }

    while (ptr_queue.size() != 1) {
      IntVectorPtr f_v_ptr = ptr_queue.front();
      ptr_queue.pop();
      IntVectorPtr s_v_ptr = ptr_queue.front();
      ptr_queue.pop();

      outf << "Process 0: I'm going to merge: " << vtos(*f_v_ptr) << " and : " << vtos(*s_v_ptr)
           << std::endl;

      auto temp = std::make_shared<IntVector>(f_v_ptr->size() + s_v_ptr->size());
      std::merge(f_v_ptr->begin(), f_v_ptr->end(), s_v_ptr->begin(), s_v_ptr->end(), temp->begin());
      ptr_queue.push(temp);
      outf << "ptr_queue.back() = " << vtos(*ptr_queue.back()) << std::endl;
      
      outf << "new ptr_queue size = " << ptr_queue.size() << std::endl;
    }
    return *ptr_queue.back();
  } else {
    MPI_Send(local_vec.data(), local_vec.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return local_vec;
}
