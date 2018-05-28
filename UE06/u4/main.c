#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <wait.h>

void printPID() {
    printf("PID: %d, PPID: %d", getpid(), getppid());
}

int main(int argc, char** argv) {
    if (argc < 2) return EXIT_FAILURE;
    int childs = atoi(argv[1]);

    printPID();
    printf("\nEs werden jetzt %d child prozesse erzeugt ...", childs);

    for (int i = 0; i < childs; i++) {
        printf("%d: ", i);
        printPID();
        printf("\n");
        int isParent = fork();
        if (isParent) {
            waitpid(isParent, NULL, 0);
            break;
        }
    }

    return 0;
}