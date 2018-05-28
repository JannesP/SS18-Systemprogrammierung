#include <stdio.h>
#include <zconf.h>
#include <libgen.h>

int main(int argc, char** argv) {
    printf("%s\n", basename(argv[0]));

    for (int i = 0; i < 20; i++) {
        printf("2 ");
        fflush(stdout);
        usleep(100000);
    }
    printf("\n");

    return 0;
}