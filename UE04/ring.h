#ifndef RING_H_INCLUDED
#define RING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//-------------------GLOBALS----------------------------------------------------

const int maxAnzahlRingElemente = 50;

typedef struct
{
    char* puffer;
    int maxPuffer;
    int readIndex;
    int writeIndex;
    pthread_mutex_t mutex;
    pthread_cond_t condWrite;
    pthread_cond_t condRead;
} RingTy;


//-------------------getNewRing-------------------------------------------------
RingTy* getNewRing()
{
    RingTy* pRing = malloc(sizeof(RingTy));

    if (pthread_mutex_init(&pRing->mutex, NULL) != 0) {
        printf("Error creating mutex.");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&pRing->condWrite, NULL) != 0) {
        printf("Error creating condWrite.");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&pRing->condRead, NULL) != 0) {
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
    pthread_mutex_lock(&(pRing->mutex));

    while(next(pRing->writeIndex, pRing) == pRing->readIndex) // Puffer voll
    {
        pthread_cond_wait(&pRing->condRead, &pRing->mutex);
    }

    pRing->writeIndex = next(pRing->writeIndex, pRing);
    pRing->puffer[pRing->writeIndex] = data;

    pthread_mutex_unlock(&pRing->mutex);
    pthread_cond_signal(&pRing->condWrite);
}

//-------------------dequeue----------------------------------------------------
// Element an Position readIndex entnehmen:
char dequeue(RingTy* pRing)
{
    pthread_mutex_lock(&pRing->mutex);


    while(pRing->writeIndex == pRing->readIndex) // Puffer leer
    {
        pthread_cond_wait(&pRing->condWrite, &pRing->mutex);
        //return pRing->puffer[0]; // um den Compiler zu beruhigen
    }

    pRing->readIndex = next(pRing->readIndex, pRing);

    char erg = pRing->puffer[pRing->readIndex];
    pRing->puffer[pRing->readIndex] = '-';

    pthread_mutex_unlock(&pRing->mutex);
    pthread_cond_signal(&pRing->condRead);
    return erg;
}

#endif // RING_H_INCLUDED
