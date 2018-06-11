//-----------------------------------------------------------------------------
// fifoA.c
//
// POSIX FIFO (named pipe) Beispiel
//
// - Die Prozesse fifoA unf fifoB kommunizieren über eine named pipe.
// - Dies ist der schreibende Prozess.
//
// - SIGINT (Strg-C) wird behandelt, um die pipe zu schließen.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//         => fifoA und fifoB müssen separat gestartet werden
//         => es können mehrere fifoA- und fifoB-Prozesse gestartet werden
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// gcc fifoA.c -o fifoA -Wall
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "schlafzufaellig.h"


char* meinFifoFile = "/tmp/fifoAB";

void meinSIGINT_Handler();
void registriere_SIGINT();

//-----------------------------------------------------------------------------
int main()
{
    registriere_SIGINT();
    initSchlafZufaellig();

    // FIFO-special-file erstellen:
    // (Fehler "existierende pipe" nicht beachten)
    if((mkfifo(meinFifoFile, 0644) == -1) && errno != EEXIST)    // rw- r-- r--
        perror("Fehler in fifoA");

    // FIFO-special-file zum Schreiben öffnen:
    int fDesc = open(meinFifoFile, O_WRONLY);

    char c[1];
    while(1)
    {
        for(c[0] = 'A'; c[0] <= 'Z'; c[0]++)
        {
            write(fDesc, c, 1);    // Anzahl=1 --> ohne \0
            printf("%s", c);
            fflush(stdout);

            schlafZufaellig(100, 400);
        }
        printf("\n");
        fflush(stdout);
    }

    return 0;
}

//-----------------------------------------------------------------------------
void meinSIGINT_Handler()   // Strg-C  --> FIFO-file loeschen
{
    puts("fifoA: tschuess\n");
    fflush(stdout);

    // FIFO-special-file entfernen:
    if(unlink(meinFifoFile) == -1)
        perror("Fehler in fifoA");

    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
void registriere_SIGINT()
{
    // Handler meinSIGINT_Handler fuer Strg-C SIGINT registrieren-------:
    struct sigaction sigact01;

    sigact01.sa_handler = meinSIGINT_Handler;
    sigact01.sa_flags = 0;
    sigemptyset(&sigact01.sa_mask);

    if (sigaction(SIGINT, &sigact01, NULL) == -1)    // Registrierung
    {
        perror("FEHLER: sigaction fehlgeschlagen\n");
        exit(1);
    }
}
