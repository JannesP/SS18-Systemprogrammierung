//-----------------------------------------------------------------------------
// fifoB.c
//
// POSIX FIFO (named pipe) Beispiel
//
// - Die Prozesse fifoA unf fifoB kommunizieren über eine named pipe.
// - Dies ist der lesende Prozess.
//
// - SIGINT (Strg-C) wird behandelt, um die pipe zu schließen.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//         => fifoA und fifoB müssen separat gestartet werden
//         => es können mehrere fifoA- und fifoB-Prozesse gestartet werden
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// gcc fifoB.c -o fifoB -Wall
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
        perror("Fehler in fifoB");

    // FIFO-special-file zum Lesen öffnen:
    int fDesc = open(meinFifoFile, O_RDONLY);


    // aus FIFO lesen
    // (der Prozess blockiert, bis ein Prozess die pipe zum Schreiben öffnet)
    char puffer[100];

    int anzahlZeichen;
    do
    {
        schlafZufaellig(100, 1000);
//        schlafZufaellig(1, 10);   // Variante

        anzahlZeichen = read(fDesc, puffer, 100);   // blockiert, falls pipe leer

        puffer[anzahlZeichen] = '\0';   // mit '\0' abschliessen => puffer wird string
        printf("fifoB hat diese %d Zeichen gelesen: \"%s\"\n", anzahlZeichen, puffer);
    }
    while (anzahlZeichen > 0);  // terminieren, falls Schreiber nicht mehr existiert
 //   while (1);        // Variante

    return 0;
}

//-----------------------------------------------------------------------------
void meinSIGINT_Handler()   // Strg-C  --> FIFO-file loeschen
{

    puts("fifoB: tschuess\n");
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
