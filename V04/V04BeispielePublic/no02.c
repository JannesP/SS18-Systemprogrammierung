//------------------------------------------------------------------------------
// .../V04/V04Beispiele/no02/no02.c
//
// Drei threads versuchen den globalen Zaehler count zu inkrementieren.
//
// Der Zugriff auf den kritischen Bereich wird mit einem Mutex geschuetzt.
//
//
// gcc no02.c -o no02 -Wall -lpthread
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//------------------------------------------------------------------------------
// globaler Mutex:
pthread_mutex_t muhteks;

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
    pthread_mutex_init(&muhteks, NULL);



    pthread_t faden1, faden2, faden3;

    pthread_create(&faden1, NULL, addOne, "faden1");
    pthread_create(&faden2, NULL, addOne, "faden2");
    pthread_create(&faden3, NULL, addOne, "faden3");

    pthread_join(faden1, NULL);
    pthread_join(faden2, NULL);
    pthread_join(faden3, NULL);

    pthread_mutex_destroy(&muhteks);   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
        pthread_mutex_lock(&muhteks);   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        localCount = count;

        waitBusy(10000);

        localCount++;
        count = localCount;

        // globalen Zaehlerstand ausgeben:
        printf("%s: count = %4d\n", threadName, count);
        fflush(stdout);

        pthread_mutex_unlock(&muhteks);   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

