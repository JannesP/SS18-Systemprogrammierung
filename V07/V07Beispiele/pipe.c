//-----------------------------------------------------------------------------
// pipe.c
//
// Ein child-Prozess sendet seinem parent-Prozess einen string über
// eine unbenannte pipe.
//
// gcc pipe.c -o pipe -Wall
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    // array für die file-Deskriptoren der beiden pipe-Enden:
    int fileDesc[2];

    // read-Puffer:
    char puffer[100];

    // die Pipe instanziieren:
    pipe(fileDesc);

    if (fork() == 0)    // child
    {
        // in Schreib-Ende schreiben:
        write(fileDesc[1], "Moin Vadder!", 13); // 13 Bytes incl. \0
        exit(EXIT_SUCCESS);
    }
    else
    {
        // am Lese-Ende lesen
        // (read blockiert, falls noch keine Daten vorhanden sind):
        read(fileDesc[0], puffer, 100); // max. 100 Bytes
        printf("Sohn hat mir \"%s\" geschickt.\n", puffer);
    }

    wait(NULL);

    return 0;
}
