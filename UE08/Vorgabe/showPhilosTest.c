//------------------------------------------------------------------------------
// showPhilosTest.c
//
// - Das Programm schreibt ins shared memory des showPhilo-Programms,
//   um es zu testen.
// - Die Funktion initSharedMem ist identisch zu der in showPhilos.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ACHTUNG: DER INITIALWERT DES SEMAPHORS WIRD NUR ÜBERNOMMEN, WENN EIN NEUER
//          SEMAPHOR ERZEUGT WIRD. EXISTIERENDE - PERSISTENTE - SEMAPHOREN
//          KÖNNEN NOCH GESPERRT SEIN -> PROZESS BLOCKIERT BEIM ERSTEN sem_wait
//          --> showPhilos unbedingt mit Strg-C oder ESC beenden, der Semaphor
//              wird dann entfernt
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// gcc -Wall showPhilosTest.c -o showPhilosTest -lpthread
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>

#define pi 3.14159265358979323846
#define anzahlPhilos 5


//------------------------------------------------------------------------------
// Zeiger in shared memory arrays (werden in initSharedMem initialisiert):
int* shmPhiloStates;   // 0:denkt, 1:isst
int* shmGabelStates;   // 0:frei,  1:belegt

// -----------------------------------------------------------------------------
void initSharedMem();

// -----------------------------------------------------------------------------
// POSIX:SEM named-semaphor Name:
char* shmSemaphorName = "/fourtytwo";   // KEIN filename, nur identifier
sem_t* shmSemaphorP;

// -----------------------------------------------------------------------------
int main()
{
    initSharedMem();

    // named-Semaphor erzeugen (shmSemaphorP ist ein pointer):
    shmSemaphorP = sem_open(shmSemaphorName, O_CREAT, 0644, 1); // 1 -> frei
    if(shmSemaphorP == SEM_FAILED)
    {
        printf("errno: %d \n", errno);
        if(errno == EEXIST)
            printf("errno == EEXIST\n");

        fflush(stdout);
    }

    int sleepMs = 1000;              // update-Frequenz in Millisekunden
    int i;
    int count = 0;

    while(1)
    {
        printf("heartbeat %d\n", ++count);

        usleep(sleepMs * 1000);

        // ins shm schreiben:

        for( i = 0; i < anzahlPhilos; i++)
        {
            sem_wait(shmSemaphorP);         // P-Operation
            shmPhiloStates[i] = 1;          // -> essen
            sem_post(shmSemaphorP);         // V-Operation

            usleep(250 * 1000);
        }

        usleep(500 * 1000);

        for( i = 0; i < anzahlPhilos; i++)
        {
            sem_wait(shmSemaphorP);         // P-Operation
            shmPhiloStates[i] = 0;          // -> denken
            sem_post(shmSemaphorP);         // V-Operation

            usleep(250 * 1000);
        }

        usleep(500 * 1000);

        for( i = 0; i < anzahlPhilos; i++)
        {
            sem_wait(shmSemaphorP);         // P-Operation
            shmGabelStates[i] = 1;          // -> belegen
            sem_post(shmSemaphorP);         // V-Operation

            usleep(500 * 1000);

            sem_wait(shmSemaphorP);         // P-Operation
            shmGabelStates[i] = 0;          // -> freigeben
            sem_post(shmSemaphorP);         // V-Operation

            usleep(250 * 1000);
        }

        usleep(500 * 1000);

    }

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
void initSharedMem()
{
    char* meinShmFile = "/tmp/philoShm";

    // meinShmFile erzeugen, falls nicht existent:
    open(meinShmFile, O_RDWR | O_CREAT, 0644);  // rw-r--r--

    key_t key;
    int shmId;
    int* shmStartAdr;

    key = ftok(meinShmFile, 42);

    // zwei int-arrays: shmPhiloStates und shmGabelStates:
    size_t shmGroesse = 2 * anzahlPhilos * sizeof(int);
    shmId = shmget(key, shmGroesse, 0644 | IPC_CREAT);  // rw-r--r--

    shmStartAdr = shmat(shmId, NULL, 0); // NULL -> OS wählt Adresse

    // Beide arrays hintereinander:
    shmPhiloStates = (int*) &shmStartAdr[0];   // 0:denkt, 1:isst
    shmGabelStates = (int*) &shmStartAdr[5];   // 0:frei,  1:belegt
}

