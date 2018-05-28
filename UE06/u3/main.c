#include <stdio.h>
#include <zconf.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int isParent = fork();

    if (isParent == -1) return EXIT_FAILURE;

    int pid = getpid();
    int ppid = getppid();

    char* charsToPrint;
    if (isParent) {
        charsToPrint = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        waitpid(isParent, NULL, 0);
    } else {
        charsToPrint = "abcdefghijklmnopqrstuvwxyz";
    }

    printf("PID: %d, PPID: %d\n", pid, ppid);

    for (int i = 0; i < 26; i++) {
        printf("%c", charsToPrint[i]);
        usleep(50000);
        fflush(stdout);
    }
    printf("\n");

    printf("fertig!\n");
    return 0;
}