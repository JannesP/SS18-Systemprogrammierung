#ifndef RING_H_INCLUDED
#define RING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

//-------------------GLOBALS----------------------------------------------------

const int maxAnzahlRingElemente = 50;

typedef struct
{
    char* puffer;
    int maxPuffer;
    int readIndex;
    int writeIndex;
    sem_t semIndex;
    sem_t semFree;
    sem_t semUsed;
} RingTy;


//-------------------getNewRing-------------------------------------------------
RingTy* getNewRing()
{
    RingTy* pRing = malloc(sizeof(RingTy));

    if (sem_init(&pRing->semIndex, 0, 1) != 0) {
        printf("Error creating mutex.");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&pRing->semFree, 0, maxAnzahlRingElemente) != 0) {
        printf("Error creating condWrite.");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&pRing->semUsed, 0, 0) != 0) {
        printf("Error creating condRead.");
        exit(EXIT_FAILURE);
    }

    pRing->readIndex  = 0;
    pRing->writeIndex = 0;

    // ein Element bleibt ungenutzt:
    pRing->maxPuffer = maxAnzahlRingElemente + 1;

    pRing->puffer = malloc(sizeof(char) * pRing->maxPuffer);

    // alle Plaetze mit '-' initialisieren:
    int i;
    for(i = 0; i < pRing->maxPuffer; i++)
        pRing->puffer[i] = '-';

    return pRing;
}

//-------------------next-------------------------------------------------------
// Positionsindex rotieren (nur berechnen, noch nicht aendern!):
int next(int i, RingTy* pRing)
{
    return (i+1) % pRing->maxPuffer;
}

//-------------------enqueue----------------------------------------------------
// Element an Position writeIndex einfuegen:
void enqueue(char data, RingTy* pRing)
{
    sem_wait(&pRing->semFree);

    sem_wait(&pRing->semIndex);
    pRing->writeIndex = next(pRing->writeIndex, pRing);
    pRing->puffer[pRing->writeIndex] = data;
    sem_post(&pRing->semIndex);

    sem_post(&pRing->semUsed);
}

//-------------------dequeue----------------------------------------------------
// Element an Position readIndex entnehmen:
char dequeue(RingTy* pRing)
{
    sem_wait(&pRing->semUsed);

    sem_wait(&pRing->semIndex);
    pRing->readIndex = next(pRing->readIndex, pRing);
    char erg = pRing->puffer[pRing->readIndex];
    pRing->puffer[pRing->readIndex] = '-';
    sem_post(&pRing->semIndex);

    sem_post(&pRing->semFree);
    return erg;
}

#endif // RING_H_INCLUDED
