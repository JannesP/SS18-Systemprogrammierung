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
// gcc no01Vorgabe.c -o no01Vorgabe -Wall -lpthread
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "ring.h"

//------------------------------------------------------------------------------
// Parametertyp fuer thread-functions:
typedef struct
{
    int delayMs; // Verzoegerung in Millisekunden
    RingTy* pRing;
} ThreadFuncParaTy;

void* erzeuge(void* pThreadFuncPara);
void* verbrauche(void* pThreadFuncPara);


//-----------------------------------------------------------------------------
int main()
{
    // Queue erzeugen:
    RingTy* pQ = getNewRing();

    pthread_t erzeuger, verbraucher;

    // thread-functions-Parameter:
    ThreadFuncParaTy tfpErzeuger, tfpVerbraucher;
    tfpErzeuger.delayMs    = 10;
    tfpVerbraucher.delayMs =  20;
    tfpErzeuger.pRing    = pQ;
    tfpVerbraucher.pRing = pQ;

    pthread_create(&erzeuger, NULL, erzeuge, &tfpErzeuger);
    pthread_create(&verbraucher, NULL, verbrauche, &tfpVerbraucher);

    while (1) {
        pthread_mutex_lock(&pQ->mutex);
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
        pthread_mutex_unlock(&pQ->mutex);
        usleep(1000 * 50);
    }

    pthread_join(erzeuger, NULL);
    pthread_join(verbraucher, NULL);

    return 0;
}


//-----------------------------------------------------------------------------
void* erzeuge(void* pThreadFuncPara)
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
