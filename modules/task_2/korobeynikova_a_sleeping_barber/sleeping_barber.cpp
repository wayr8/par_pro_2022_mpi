// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_2/korobeynikova_a_sleeping_barber/sleeping_barber.h"

#include <mpi.h>
#include <string>
#include <vector>
#include <fstream>

void sendClientToBarber(int clientNum) {
  // отправим клиента к парикмахеру
  MPI_Send(&clientNum, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD);
}

std::ofstream out_room("C:\\Users\\User\\Desktop\\out_room.txt");
#define INFO(mes) out_room << mes << std::endl

std::ofstream out_client("C:\\Users\\User\\Desktop\\out_client.txt");
#define INFO_CLIENT(mes) out_client << mes << std::endl

std::ofstream out_barber("C:\\Users\\User\\Desktop\\out_barber.txt");
#define INFO_BARBER(mes) out_barber << mes << std::endl

void waitingRoom(int waitingRoomSize) {
  int servedCust = 0;
  CustomerQueue queue(waitingRoomSize);

  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  std::vector<std::pair<MPI_Request *, int> > requests(
      waitingRoomSize, std::make_pair(nullptr, -1));

  INFO("Begin");

  while(true) {
    // в этом цикле заведём request для каждого клиента
    for (int i = 0; i < waitingRoomSize; ++i) {
      auto &requestPtr = requests.at(i).first;
      auto &procNum = requests.at(i).second;
      if (requestPtr == nullptr) {
        MPI_Request *request = new MPI_Request;
        MPI_Irecv(&procNum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                  request);
        requestPtr = request;
      }
    }
    INFO("all chairs are waiting customers");
    
    bool recvWasCompleted = false;
    double start = MPI_Wtime();
    while (!recvWasCompleted) {
      double end = MPI_Wtime();
      if (end - start > 1) { // клиенты долго не появляются
        INFO("timeout");
        for (auto & requestPtr_procNum : requests) { // удалим все request
          auto &requestPtr = requestPtr_procNum.first;
          MPI_Cancel(requestPtr);
          MPI_Request_free(requestPtr);
          delete requestPtr;          
          requestPtr = nullptr;
        }
        // говорим парикмахеру, что их больше не будет
        INFO("timeout: send begin");
        sendClientToBarber(no_more_clients);
        INFO("timeout: send end");
        return;
      }
      for (auto &requestPtr_procNum : requests) {
        auto &requestPtr = requestPtr_procNum.first;
        auto &procNum = requestPtr_procNum.second;
        MPI_Status status;
        int flag;
        MPI_Test(requestPtr, &flag, &status);
        // проверяем, что данные от клиента с номером procNum пришли
        if (flag != 0) {
          INFO("handle request");
          int response = take_your_sit;
          try {
            queue.push(procNum);  // пытаемся пропихнуть его в очередь
          } catch (const std::exception &) {
            response = no_place;
          }
          ++servedCust;
          // отправим этому клиенту,можно ли ему сесть в комнате ожидания
          MPI_Send(&response, 1, MPI_INT, procNum, 0, MPI_COMM_WORLD);
          delete requestPtr;
          requestPtr = nullptr;
          procNum = -1;
          recvWasCompleted = true;
          INFO("handle request end");
          //break;
        }
      }
    }
    int clientToBarber = queue.front();
    sendClientToBarber(clientToBarber);
    queue.pop();    
  }
}
void customer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int response;
  MPI_Status status;
  MPI_Request request;
  MPI_Isend(&rank, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD, &request);
  
  {
    std::string message("client ");
    message += std::to_string(rank);
    message += " wait for recv 1: begin";
    INFO_CLIENT(message);
  }

  MPI_Recv(&response, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD, &status);

  {
    std::string message("client ");
    message += std::to_string(rank);
    message += " wait for recv 1: end";
    INFO_CLIENT(message);
  }

  if (response == take_your_sit) {
    int res;

    {
      std::string message("client ");
      message += std::to_string(rank);
      message += " wait for recv 2: begin";
      INFO_CLIENT(message);
    }

    MPI_Recv(&res, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD, &status);

    {
      std::string message("client ");
      message += std::to_string(rank);
      message += " wait for recv 2: end";
      INFO_CLIENT(message);
    }
  }
}
int barber() {
  int servedСust = 0;
  MPI_Status status;
  int clientNum = 0;
  while (clientNum != no_more_clients) {    
    MPI_Recv(&clientNum, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD,
             &status);    
    // можно устроить некоторое время ожидания для симуляции стрижки
    if (clientNum != no_more_clients) {
      int res = you_got_a_beautiful_haircut;
      MPI_Send(&res, 1, MPI_INT, clientNum, 0, MPI_COMM_WORLD);
      ++servedСust;
    }    
  }
  return servedСust;
}
