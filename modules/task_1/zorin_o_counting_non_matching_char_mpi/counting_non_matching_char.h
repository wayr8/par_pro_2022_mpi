#ifndef MODULES_TASK_1_COUNTING_NON_MATCHING_CHAR_MPI_H
#define MODULES_TASK_1_COUNTING_NON_MATCHING_CHAR_MPI_H

#include <string>

std::string getRandomString(size_t size);
int countNonMatchingCharSequential(const std::string &str,
                                   const std::string &compare_str);
int countNonMatchingCharParallel(const std::string &global_str,
                                 const std::string &global_compare_str);


#endif // MODULES_TASK_1_COUNTING_NON_MATCHING_CHAR_MPI_H
