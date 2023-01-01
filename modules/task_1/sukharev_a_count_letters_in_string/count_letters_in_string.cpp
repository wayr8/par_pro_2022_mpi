// Copyright 2022 Sukharev Artem
#include <mpi.h>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/sukharev_a_count_letters_in_string/count_letters_in_string.h"

void printString_and_Letter(const std::string& name, const std::string& localString,const char letter) {
    std::cout << name << " : ";
    std::cout << "string = \""<<localString<<"\", ";
    std::cout << "letter = "<<letter<<"\n";
}

std::string getRandomString(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::string randomString(size);
    for (int i = 0; i < size; i++) {
        vec[i] = static_cast<char>(static_cast<int>('a')+gen()%26);
    }
    return randomString;
}

char getRandomLetter() {
    std::random_device dev;
    std::mt19937 gen(dev());
    return static_cast<char>(static_cast<int>('a')+gen()%26);
}

int countLettersSequential(std::string localString,const char letter) {
    const int size = vec.size();
    int amount = 0;
    for(int i = 0; i < size; i++) {
        if(localString[i]==letter)
            amount++;
    }
    return amount;
}

int countLettersParallel(const std::string& globalString, const char letter) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int amount=0;
    if(comm_size==1){
        return countLettersSequential(globalString,letter);
    }
    


}
