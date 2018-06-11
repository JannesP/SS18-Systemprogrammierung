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

void eat(int index) {
    int startLeft = index % 2 == 0;

    sem_wait(shmSemaphorP);         // P-Operation
    shmPhiloStates[index] = 1;          // -> essen
    sem_post(shmSemaphorP);         // V-Operation

    sem_wait(shmSemaphorP);         // P-Operation
    shmGabelStates[index] = 1;          // -> eigene belegen
    shmGabelStates[(((index - 1) + 5) % 5)] = 1;          // -> rechts von sich
    sem_post(shmSemaphorP);         // V-Operation
}

void philo(int index) {


    while (1 < 2) {
        eat(index);

    }
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