//-----------------------------------------------------------------------------
//
// hMan.c     (Demoprogramm zur hManLib.h)
//
//
// gcc hMan.c -o hMan -Wall
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "hManLib.h"
#include "pthread.h"

void* draw(void);
void meinSIGINT_Handler(void);
void registriere_SIGINT(void);

char* meinFifoFile = "/tmp/fifoAB";
char currentAction = '\0'; // init empty char
pthread_mutex_t mutex;

// hman variables
int hManX = 40;
int hManY = 12;
int hManOrientation = 1;

int main()
{
    char puffer[100];
    pthread_t drawThread;

    registriere_SIGINT();

    // FIFO
    if((mkfifo(meinFifoFile, 0644) == -1) && errno != EEXIST)    // rw- r-- r--
        perror("Fehler in fifoB");


    // FIFO-special-file zum Lesen öffnen:
    int fDesc = open(meinFifoFile, O_RDONLY);

    machRot();

    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&drawThread, NULL, draw, NULL) != 0)
    {
        puts("Fehler: pthread_create return code != 0\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        //char c = liesCharVonStdin();
        int anzahlZeichen = read(fDesc,puffer,100);
        pthread_mutex_lock(&mutex);
        switch(puffer[0]){
            case '\0':
                // NOTHING!
                break;
            case 'j':
                hManX--;
                break;
            case 'k':
                hManX++;
                break;
            case 'i':
                hManY--;
                break;
            case 'm':
                hManY++;
                break;
            case 'f':
                // langsamer
                break;
            case 'F':
                // schneller
                break;
            case 'c':
                // farbe wechseln
            case 'Q':
                // quit
                break;
        }
        pthread_mutex_unlock(&mutex);
        usleep(500 * 10);
    }
    return 0;
}

void* draw(){
    initDisplay();

    while(1)
    {
        // lock mutex, read hManData
        pthread_mutex_lock(&mutex);
        int x = hManX;
        int y = hManY;
        int or = hManOrientation;
        pthread_mutex_unlock(&mutex);

        clearDisplay();
        zeichneUpDownManAt( x, y, or);
        usleep(500 * 1000);
    }
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
