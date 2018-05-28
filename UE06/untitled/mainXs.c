#include <stdio.h>
#include <zconf.h>
#include <libgen.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) return EXIT_FAILURE;
    int x = atoi(argv[1]);
    if (x == 0) return EXIT_FAILURE;

    printf("%s\n", basename(argv[0]));

    time_t endTime = time(0) + x;

    int count = 0;
    while (endTime >= time(0)) {
        printf("x ");
        usleep(100000);
        if (++count % 20 == 0) printf("\n");
        fflush(stdout);
    }

    printf("\n");

    return 0;
}
