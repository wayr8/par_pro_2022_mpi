#pragma once
// Copyright 2022 Korobeynikova Alisa
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <queue>

#define barberProc 0
#define waitingRoomProc 1
#define no_place 0
#define take_your_sit 1
#define you_got_a_beautiful_haircut 2
#define no_more_clients -1

class CustomerQueue : public std::queue<int> {
 private:
  int fixedSize;

 public:
  CustomerQueue(int _size) : std::queue<int>(), fixedSize(_size) {}
  void push(const int &value) {
    if (size() == fixedSize) {
      throw std::exception("there is no place for you");
    }
    std::queue<int>::push(value);
  }
};
void sendClientToBarber(int clientNum);
void waitingRoom(int waitingRoomSize);
void customer();
int barber();
#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
