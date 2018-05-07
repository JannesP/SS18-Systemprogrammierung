//------------------------------------------------------------------------------
// .../V04/V04Beispiele/no01/no01.c
//
// Drei threads versuchen den globalen Zaehler count zu inkrementieren.
//
// Test: Warteschleife waitBusy(....) auskommentieren  !!!!!!!!!!!!!!!!!!!!!!!!
//
// gcc no01.c -o no01 -Wall -lpthread
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//------------------------------------------------------------------------------
// globaler Zaehler:
int count = 0;

//------------------------------------------------------------------------------
// thread-function:
void* addOne(void*);

//------------------------------------------------------------------------------
// aktive Warteschleife:
void waitBusy(int anzahlLoops);

//------------------------------------------------------------------------------
int main()
{
    pthread_t faden1, faden2, faden3;

    pthread_create(&faden1, NULL, addOne, "faden1");
    pthread_create(&faden2, NULL, addOne, "faden2");
    pthread_create(&faden3, NULL, addOne, "faden3");

    pthread_join(faden1, NULL);
    pthread_join(faden2, NULL);
    pthread_join(faden3, NULL);

    return 0;
}


//------------------------------------------------------------------------------
void* addOne(void* pthreadName)
{
    char* threadName = (char*)pthreadName;

    int localCount;

    int i;
    for(i = 1; i <= 100; i++)
    {
        localCount = count;

        waitBusy(10000);				//!!!!!!!!!!!!!!!!!!!!!!

        localCount++;
        count = localCount;

        // globalen Zaehlerstand ausgeben:
        printf("%s: count = %4d\n", threadName, count);
        fflush(stdout);
    }
    return NULL;
}


// -----------------------------------------------------------------------------
void waitBusy(int anzahlLoops)
{
    // volatile (speicherresident) -> wird nicht wegoptimiert:
    volatile double x = 1.111;

    int i;
    for(i = 1; i <= anzahlLoops; i++)
        x = x * 1.001;
}

