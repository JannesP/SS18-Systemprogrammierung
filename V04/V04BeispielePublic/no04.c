//------------------------------------------------------------------------------
// .../V04/V04Beispiele/no03/no04.c
//
// Zwei threads werden mit einer Bedingungsvariablen synchronisiert.
//
//
// gcc no04.c -o no04 -Wall -lpthread
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//------------------------------------------------------------------------------
// globaler Mutex und Bedingungsvariable:
pthread_mutex_t muh;
pthread_cond_t condVar;                 //!!!!!!!!!!!!!!

//------------------------------------------------------------------------------
// globaler Zaehler:
int count = 10;

//------------------------------------------------------------------------------
// thread-functions:
void* machWattA(void*);
void* machWattB(void*);

//------------------------------------------------------------------------------
int main()
{
    pthread_mutex_init(&muh, NULL);
    pthread_cond_init(&condVar, NULL);  //!!!!!!!!!!!!!!

    pthread_t faden1, faden2;

    pthread_create(&faden1, NULL, machWattA, NULL);
    pthread_create(&faden2, NULL, machWattB, NULL);

    pthread_join(faden1, NULL);
    pthread_join(faden2, NULL);

    return 0;
}


//------------------------------------------------------------------------------
void* machWattA(void* p)
{
    pthread_mutex_lock(&muh);

    while(count < 42)
        pthread_cond_wait(&condVar, &muh);  // --> blockiert UND unlock-Mutex

    printf("\nDas Ergebnis lautet: %d\n", count);
    exit(EXIT_SUCCESS);

    pthread_mutex_unlock(&muh);

    return NULL;
}

//------------------------------------------------------------------------------
void* machWattB(void* p)
{
    while(1)
    {
        // lock, um den krit. Bereich zu schuetzen.
        // Fuer cond-signal nicht erforderlich:
        pthread_mutex_lock(&muh);

        count++;

        if(count >= 42)
            pthread_cond_signal(&condVar);  // --> machWattA wecken UND locked Mutex "mitnehmen"

        pthread_mutex_unlock(&muh); // unlock krit. Bereich

        printf(". ");
        fflush(stdout);
        usleep(100000);
    }

    return NULL;
}

