#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <wait.h>
#include <fcntl.h>

void printPID() {
    printf("PID: %d, PPID: %d", getpid(), getppid());
}

int main(int argc, char** argv) {

    int isParent1 = fork();


    if (isParent1) {
        int isParent2 = fork();
        if (isParent2) {
            waitpid(isParent1, NULL, 0);
            waitpid(isParent2, NULL, 0);
            printf("fertig!");
        } else {
            execl("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u4/cmake-build-debug/make2s", "make2s", (char*)NULL);
        }
    } else {
        int flags = O_WRONLY | O_CREAT | O_TRUNC;
        mode_t permission = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fdOut = open("out.txt", flags, permission);
        close(STDOUT_FILENO);
        int fdNew = dup(fdOut);
        execl("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u4/cmake-build-debug/makeXs", "makeXs", "5", (char*)NULL);
        close(fdOut);
        FILE* fStreamNew = fdopen(fdNew, "w+");
        fflush(fStreamNew);
        close(fdNew);
    }

    return 0;
}
