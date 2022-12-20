// Copyright 2022 Shokurov Daniil
#ifndef MODULES_TASK_1_SHOKUROV_D_CHECK_ORDER_CHECK_ORDER_H
#define MODULES_TASK_1_SHOKUROV_D_CHECK_ORDER_CHECK_ORDER_H

#include <vector>
#include <string>

using namespace std;

void get_random_string(string& str, size_t n);
void scatter_string(string& str1);
int check_order_single_process(size_t n, string& a, string& b);
void addNull(string& str, int count);
int getOrder(string& str1, string& str2);
#endif  // MODULES_TASK_1_SHOKUROV_D_CHECK_ORDER_CHECK_ORDER_H
