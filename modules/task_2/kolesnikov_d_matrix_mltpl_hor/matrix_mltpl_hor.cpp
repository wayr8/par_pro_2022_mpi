
#include "../../../modules/task_2/kolesnikov_d_matrix_mltpl_hor/matrix_mltpl_hor.h"

vector<int> GenRndMtrx(int size_x, int size_y) {
  std::random_device dev;
  std::mt19937_64 gen(dev());
  vector<int> result(size_x * size_y);
  for (int i = 0; i < size_x * size_y; ++i) {
    result[i] = gen() % 100;
  }
  return result;
}

int CoorldLin(int x, int y, int size_x) {
  return y * size_x + x;
}

vector<int> MatrixMtlplSeq(
    vector<int>& a,
    int a_size_x,
    int a_size_y,
    vector<int>& b,
    int b_size_x,
    int b_size_y) {
  if (a_size_x == 0 || a_size_y == 0 || b_size_x == 0 b_size_y == 0 ) {
    return vector<int>();
  }
  int tmp;
  vector<int> c(a_size_y, b_size_x);
  for (int i = 0; i < a_size_y; i++) {
    for (int j = 0; j < b_size_x; j++) {
      tmp = 0;
      for (int k = 0; k < a_size_x; k ++) {
        c[i * b_size_x + j] = a[i * a_size_x + k] * b[k * b_size_x + j];
      }
    }
  }
  return c;
}

vector<int> MatrixMtlplPrl(
    vector<int>& a,
    int a_size_x,
    int a_size_y,
    vector<int>& b,
    int b_size_x,
    int b_size_y) {



  int n_process;
  int index;
  int tmp;
  int proc_num, rank;

  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  int proc_part_a = a_size_y / proc_num;
  int part_a = proc_part_a * a_size_x;
  int proc_part_b = b_size_x / proc_num;
  int part_b = proc_part_b * b_size_y;


  vector<int> buf_a(part_a);
  vector<int> buf_b(part_b);
  vecotor<int> buf_c(proc_part_a * b_size_y);

  MPI_Scatter(a, part_a, MPI_INT, buf_a, part_a, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(b, part_b, MPI_INT, buf_b, part_b, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < proc_part_a; i++) {
    for (int j = 0; j < proc_part_b; j++) {
      tmp = 0;
      for (int k = 0; k < a_size_x; k++) {
        tmp += buf_a[i * a_size_x + k] * buf_b[j * a_size_x + k];
        buf_c[i* b_size_x + j + proc_part_a * rank] = tmp;
      } 
    }
  }


  int next_proc; 
  int prev_proc;
  next_proc = rank + 1;
  if (rank == proc_num - 1) next_proc = 0;
  prev_proc = rank - 1;
  if (rank == 0) prev_proc = proc_num - 1;

  for(n_process = 1; n_process < proc_num; n_process++) 
  {
    MPI_Sendrecv_replace(buf_b, part_b, MPI_INT, next_proc, 0, prev_proc, 0, MPI_COMM_WORLD, &Status);   // передача-принятие единого типа сообщений (рассылка B)
    tmp = 0;
    for (i = 0; i < proc_part_a; i++) {
      for(j = 0; j < proc_part_b; j++)  {
        tmp = 0;
        for(k = 0; k < a_size_x; k++) {
          tmp += buf_a[i * a_size_x + k] * buf_b[j * a_size_x + k];     
            if (rank - n_process >= 0)
              index = rank - n_process;
            else 
              index =(proc_num - n_process + rank);
            buf_C[i * b_size_x + j + index * proc_part_a] = tmp;
        }
      }
    }
  }
  MPI_Gather(buf_c, proc_part_a * b_size_x, MPI_INT, c, proc_part_a * b_size_x, MPI_INT, 0, MPI_COMM_WORLD);  
}