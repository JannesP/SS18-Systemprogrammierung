//------------------------------------------------------------------------------
// .../V04/V04Beispiele/no03/no03.c
//
// Zwei threads versuchen staendig auf stdout zu schreiben.
//
// Der Zugriff auf den kritischen Bereich wird mit ZWEI Mutexen fenhlerhaft
// geschuetzt:
//                 ==>    DEADLOCK
//
//                                Mehrfach starten !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// gcc no03.c -o no03 -Wall -lpthread
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//------------------------------------------------------------------------------
// globale Mutexe:
pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

//------------------------------------------------------------------------------
// globaler Zaehler:
int count = 0;

//------------------------------------------------------------------------------
// thread-functions:
void* sachA(void*);
void* sachB(void*);

//------------------------------------------------------------------------------
// aktive Warteschleife:
void waitBusy(int anzahlLoops);

//------------------------------------------------------------------------------
int main()
{
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_t faden1, faden2;

    pthread_create(&faden1, NULL, sachA, NULL);
    pthread_create(&faden2, NULL, sachB, NULL);

    pthread_join(faden1, NULL);
    pthread_join(faden2, NULL);

    pthread_mutex_destroy(&mutexA);
    pthread_mutex_destroy(&mutexB);

    return 0;
}


//------------------------------------------------------------------------------
void* sachA(void* p)
{
    while(1)
    {
        pthread_mutex_lock(&mutexA);

        waitBusy(10000);    // hier nicht notwendig

        pthread_mutex_lock(&mutexB);
        printf("A");
        fflush(stdout);
        pthread_mutex_unlock(&mutexB);

        pthread_mutex_unlock(&mutexA);
    }
    return NULL;
}

//------------------------------------------------------------------------------
void* sachB(void* p)
{
    while(1)
    {
        pthread_mutex_lock(&mutexB);

        waitBusy(10000);

        pthread_mutex_lock(&mutexA);
        printf("B");
        fflush(stdout);
        pthread_mutex_unlock(&mutexA);

        pthread_mutex_unlock(&mutexB);
    }
    return NULL;
}


// -----------------------------------------------------------------------------
void waitBusy(int anzahlLoops)
{
    volatile double x = 1.111;	// -> wird nicht wegoptimiert

    int i;
    for(i = 1; i <= anzahlLoops; i++)
        x = x * 1.001;
}

