// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_2/korobeynikova_a_sleeping_barber/sleeping_barber.h"

#include <mpi.h>

#include <fstream>
#include <string>
#include <vector>

void sendClientToBarber(int *message) {  // message = procNum
  // отправим клиента к парикмахеру
  MPI_Send(message, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD);
}
void sendResponseToClient(int *response, int procNum) {
  MPI_Send(response, 1, MPI_INT, procNum, 0, MPI_COMM_WORLD);
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

  INFO("Begin");
  bool barberIsFree = false;
  while (true) {
    int procNum;
    MPI_Status status;
    MPI_Recv(&procNum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    {
      std::string message("receive from ");
      message += std::to_string(procNum);
      INFO(message);
    }
    if (procNum == barberProc) {  // пришёл ответ от парикмахера - он закончил
      if (queue.empty() == false) {
        int clientToBarber = queue.front();
        sendClientToBarber(&clientToBarber);
        {
          std::string message("client ");
          message += std::to_string(clientToBarber);
          message += " goes to barber";
          INFO(message);
        }
        queue.pop();
      } else if (servedCust == procCount - 2) {
        INFO("timeout: send begin");
        int message = no_more_clients;
        sendClientToBarber(&message);
        INFO("timeout: send end");
        return;
      } else {
        barberIsFree = true;
      }
    } else {  // пришёл запрос от клиента - он хочет узнать, есть ли место в
              // очереди
      {
        std::string message("new client ");
        message += std::to_string(procNum);
        INFO(message);
      }
      int response = take_your_sit;
      try {
        queue.push(procNum);  // пытаемся пропихнуть его в очередь
      } catch (const std::exception &) {
        response = no_place;
      }
      ++servedCust;
      // отправим этому клиенту,можно ли ему сесть в комнате ожидания
      sendResponseToClient(&response, procNum);
      {
        std::string message("client ");
        message += std::to_string(procNum);
        message += (response == take_your_sit ? " is waiting for haircut"
                                              : " has to go");
        INFO(message);
      }
    }
    if (barberIsFree && queue.empty() == false) {
      int clientToBarber = queue.front();
      sendClientToBarber(&clientToBarber);
      {
        std::string message("client ");
        message += std::to_string(clientToBarber);
        message += " goes to barber";
        INFO(message);
      }
      queue.pop();
      barberIsFree = false;
    }
  }
}
void customer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int response;
  MPI_Status status;

  {
    std::string message("client ");
    message += std::to_string(rank);
    message += " is coming (send): begin";
    INFO_CLIENT(message);
  }

  double end = MPI_Wtime();
  MPI_Send(&rank, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD);
  double start = MPI_Wtime();

  {
    std::string message("client ");
    message += std::to_string(rank);
    message += " is coming (send): end";
    INFO_CLIENT(message);
  }

  {
    std::string message("client ");
    message += std::to_string(rank);
    message += " is waiting for taking a sit: begin";
    INFO_CLIENT(message);
  }

  MPI_Recv(&response, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD, &status);

  {
    std::string message("client ");
    message += std::to_string(rank);
    message += (response == take_your_sit ? " sits: end" : " goes out: end");
    INFO_CLIENT(message);
  }

  if (response == take_your_sit) {
    int res;

    {
      std::string message("client ");
      message += std::to_string(rank);
      message += " is waiting for haircut: begin";
      INFO_CLIENT(message);
    }

    MPI_Recv(&res, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD, &status);

    {
      std::string message("client ");
      message += std::to_string(rank);
      message += " got a haircut: end";
      INFO_CLIENT(message);
    }
  }
}
int barber() {
  int servedСust = 0;
  MPI_Status status;
  int clientNum = 0;
  while (clientNum != no_more_clients) {
    MPI_Send(&clientNum, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD);
    MPI_Recv(&clientNum, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD,
             &status);
    // можно устроить некоторое время ожидания для симуляции стрижки
    {
      std::string message("i begin making a haircut for client ");
      message += std::to_string(clientNum);
      INFO_BARBER(message);
    }       
    if (clientNum != no_more_clients) {
      int res = you_got_a_beautiful_haircut;
      MPI_Send(&res, 1, MPI_INT, clientNum, 0, MPI_COMM_WORLD);
      ++servedСust;
      std::string message("i finished a haircut for client ");
      message += std::to_string(clientNum);
      INFO_BARBER(message);
      clientNum = barberProc;
    }
  }
  return servedСust;
}
