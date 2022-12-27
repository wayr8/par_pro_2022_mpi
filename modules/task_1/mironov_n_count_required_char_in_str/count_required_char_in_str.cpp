  // Copyright 2022 Mironov Nikita
#include "../../../modules/task_1/mironov_n_count_required_char_in_str/count_required_char_in_str.h"

void getRandomStr(char *str, int strSize) {
    const char arr[] = "abcdefghijklmnopqrstuvwxyz";
    if (strSize <= 0)
        throw "Size can't be negative";
    for (int i = 0; i < strSize; i++) {
        str[i] = arr[rand() % (strlen(arr))];
    }
}

int countRequiredCharInProcStr(char *str, char requiredChar) {
    int charCount = 0;
    size_t strLength = strlen(str);
    for (size_t i = 0; i < strLength; i++) {
        if (str[i] == requiredChar)
            charCount++;
    }
    return charCount;
}

int countRequiredCharInStr(char *str, char requiredChar) {
    int procCount, procId;

    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    int errorCode = 0;

    if (strlen(str) == 0) {
        errorCode = 1;
    }
    MPI_Bcast(&errorCode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (errorCode != 0) {
        throw "errorCode = 1 : Str can't be empty.";
    }

    if (procCount == 1) {
        return countRequiredCharInProcStr(str, requiredChar);
    }

    int oneProcWorkAmount = strlen(str) / procCount;
    int nonDistWork = strlen(str) % procCount;

    int* oneProcWorkAmountForScatterv;
    int* scattervDispls;

    int procCharCount = 0;
    int resultCount = 0;

    MPI_Comm commForProcInUse;
    MPI_Comm commForNonUsedProc;

    if (oneProcWorkAmount == 0) {
        int color;
        if (procId < nonDistWork) {
            color = 1;
            MPI_Comm_split(MPI_COMM_WORLD, color, procId, &commForProcInUse);
            MPI_Comm_size(commForProcInUse, &procCount);
            MPI_Comm_rank(commForProcInUse, &procId);
            oneProcWorkAmountForScatterv = new int[strlen(str)];
            scattervDispls = new int[strlen(str)];
            for (int i = 0; i < strlen(str); i++) {
                oneProcWorkAmountForScatterv[i] = 1;
                scattervDispls[i] = i;
            }
        } else {
            color = 2;
            MPI_Comm_split(MPI_COMM_WORLD, color, procId, &commForNonUsedProc);
            MPI_Comm_size(commForNonUsedProc, &procCount);
            MPI_Comm_rank(commForNonUsedProc, &procId);
            return 0;
        }
    } else {
        commForProcInUse = MPI_COMM_WORLD;
        oneProcWorkAmountForScatterv = new int[procCount];
        scattervDispls = new int[procCount];
        for (int i = 0; i < procCount; i++) {
            scattervDispls[i] = i+1;
            oneProcWorkAmountForScatterv[i] = oneProcWorkAmount;
            if (i+1 == procCount && nonDistWork != 0) {
                oneProcWorkAmountForScatterv[i] =
                    oneProcWorkAmount + nonDistWork;
            }
        }
    }

    char* procStr = new char[oneProcWorkAmountForScatterv[procId]];

    if (procId == 0) {
        int messMove = 0;
        for (int proc = 1; proc < procCount; proc++) {
            messMove += oneProcWorkAmount;
            MPI_Send(&str[0] + messMove,
            oneProcWorkAmountForScatterv[proc], MPI_CHAR, proc, 1,
            commForProcInUse);
        }

        for (int i = 0; i < oneProcWorkAmountForScatterv[0]; i++) {
            procStr[i] = str[i];
        }
    }


    if (procId != 0) {
        MPI_Status status;
        MPI_Recv(&procStr[0], oneProcWorkAmountForScatterv[procId],
            MPI_CHAR, 0, 1, commForProcInUse, &status);
    }

    procCharCount = countRequiredCharInProcStr(procStr, requiredChar);

    MPI_Barrier(commForProcInUse);

    MPI_Reduce(&procCharCount, &resultCount,
        1, MPI_INT, MPI_SUM, 0, commForProcInUse);

    return resultCount;
}
