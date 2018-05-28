#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <wait.h>

void printPID() {
    printf("PID: %d, PPID: %d", getpid(), getppid());
}

int main(int argc, char** argv) {

    int isParent = fork();

    if (isParent) {
        wait(NULL);
        printf("fertig!");
    } else {
        char* argv2[] = {"make2s"};
        //execv("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u4/cmake-build-debug/make2s", argv2);
        execl("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u4/cmake-build-debug/make2s", "make2s", (char*)NULL);
    }

    return 0;
}
