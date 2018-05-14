//-----------------------------------------------------------------------------
// no01Vorgabe.c
//
// Erzeuger/Verbraucher-Problem, abgesichert mit Bedingungsvariablen.
//
// Ein Erzeuger-thread schreibt die Zeichen A bis Z in eine globale queue,
// ein Verbraucher liest aus der queue.
//
// Die Frequenzen der Erzeuger und Verbraucher sind einstellbar.
//
// gcc no01Vorgabe.c -o no01Vorgabe -Wall -lpthread -lsemaphore
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "ring.h"

//------------------------------------------------------------------------------
// Parametertyp fuer thread-functions:
typedef struct
{
    int delayMs; // Verzoegerung in Millisekunden
    RingTy* pRing;
} ThreadFuncParaTy;

void* erzeugeAZ(void* pThreadFuncPara);
void* erzeugeaz(void* pThreadFuncPara);
void* erzeuge09(void* pThreadFuncPara);
void* verbrauche(void* pThreadFuncPara);


//-----------------------------------------------------------------------------
int main()
{
    // Queue erzeugen:
    RingTy* pQ = getNewRing();

    pthread_t erzeugerAZ, erzeugeraz, erzeuger09, verbraucher0, verbraucher1, verbraucher2;

    // thread-functions-Parameter:
    ThreadFuncParaTy tfpErzeuger, tfpVerbraucher;
    tfpErzeuger.delayMs    = 10;
    tfpVerbraucher.delayMs =  20;
    tfpErzeuger.pRing    = pQ;
    tfpVerbraucher.pRing = pQ;

    pthread_create(&erzeugerAZ, NULL, erzeugeAZ, &tfpErzeuger);
    pthread_create(&erzeugeraz, NULL, erzeugeaz, &tfpErzeuger);
    pthread_create(&erzeuger09, NULL, erzeuge09, &tfpErzeuger);
    pthread_create(&verbraucher0, NULL, verbrauche, &tfpVerbraucher);
    pthread_create(&verbraucher1, NULL, verbrauche, &tfpVerbraucher);
    pthread_create(&verbraucher2, NULL, verbrauche, &tfpVerbraucher);

    while (1) {
        sem_wait(&pQ->semIndex);
        int i;
        for (i = 0; i < pQ->maxPuffer; i++) {
            printf("%c", pQ->puffer[i]);
        }
        printf("\n");
        fflush(stdout);
        if (next(pQ->writeIndex, pQ) == pQ->readIndex) {    //voll
            tfpErzeuger.delayMs = 50;
            tfpVerbraucher.delayMs = 15;
        } else if (pQ->writeIndex == pQ->readIndex) {   //leer
            tfpErzeuger.delayMs = 25;
            tfpVerbraucher.delayMs = 50;
        }
        sem_post(&pQ->semIndex);
        usleep(1000 * 50);
    }


    return 0;
}


//-----------------------------------------------------------------------------
void* erzeugeAZ(void* pThreadFuncPara)
{
    ThreadFuncParaTy* tfp = (ThreadFuncParaTy*)pThreadFuncPara;
    RingTy* pRing = tfp->pRing;

    char z;
    while(1)
    {
        for(z = 'A'; z <= 'Z'; z++)
        {
            usleep(1000 * tfp->delayMs);
            enqueue(z, pRing);
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
void* erzeugeaz(void* pThreadFuncPara)
{
    ThreadFuncParaTy* tfp = (ThreadFuncParaTy*)pThreadFuncPara;
    RingTy* pRing = tfp->pRing;

    char z;
    while(1)
    {
        for(z = 'a'; z <= 'z'; z++)
        {
            usleep(1000 * tfp->delayMs);
            enqueue(z, pRing);
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
void* erzeuge09(void* pThreadFuncPara)
{
    ThreadFuncParaTy* tfp = (ThreadFuncParaTy*)pThreadFuncPara;
    RingTy* pRing = tfp->pRing;

    char z;
    while(1)
    {
        for(z = '0'; z <= '9'; z++)
        {
            usleep(1000 * tfp->delayMs);
            enqueue(z, pRing);
        }
    }
    return NULL;
}


//-----------------------------------------------------------------------------
void* verbrauche(void* pThreadFuncPara)
{
    ThreadFuncParaTy* tfp = (ThreadFuncParaTy*)pThreadFuncPara;
    //int delayUs = 1000 * tfp.delayMs;
    RingTy* pRing = tfp->pRing;

    int i;
    while(1)
    {
        for(i = 1; i <= maxAnzahlRingElemente; i++)
        {
            usleep(1000 * tfp->delayMs);
            dequeue(pRing);
        }
    }
    return NULL;
}
