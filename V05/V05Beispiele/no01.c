//-----------------------------------------------------------------------------
// no01.c
//
// Reihenfolgensynchronisatiion mit einem Semaphor:
//
//  - main steuert einen thread mit P-und V-Operationen
//
// gcc no01.c -o no01 -Wall -lpthread
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "schlafzufaellig.h"

//------------------------------------------------------------------------------
void* machWatt();

//------------------------------------------------------------------------------
sem_t semA;

//-----------------------------------------------------------------------------
int main()
{
    pthread_t faden;

    sem_init(&semA, 0, 1); // initialisiert mit 1 -> frei

    pthread_create(&faden, NULL, machWatt, NULL);

    int i;

    // dem thread zufaellig Laufzeit geben:
    for(i = 1; i <= 25; i++)
    {
        sem_wait(&semA);        // faden-thread anhalten
        schlafZufaellig(50, 1000);
        sem_post(&semA);        // faden-thread freigeben

        usleep(10 * 1000);    // dem anderen eine Chance geben
    }

    return 0;
}


//-----------------------------------------------------------------------------
void* machWatt()
{
    char c = 'A';
    while(1)
    {
        sem_wait(&semA);      // P-Operation, auf main-Freigabe warten
        printf("%c ", c++);
        fflush(stdout);
        sem_post(&semA);      // V-Operation-> main kann weitermachen

        if(c > 'Z')
            c = 'A';
        usleep(10 * 1000);    // dem anderen eine Chance geben
    }

    return NULL;
}
