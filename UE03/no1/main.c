//-----------------------------------------------------------------------------
// no01Vorgabe.c
//
// Main-thread plus zwei konkurrierende threads.
//
// gcc no01Vorgabe.c -o no01Vorgabe -Wall -lpthread
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>		// fuer usleep



//---------------------PROTOTYPEN----------------------------------------------
void* rauf(void* arg);
void* runter(void* arg);

//-----------------------------------------------------------------------------
int main()
{
    pthread_t faden1, faden2;

    // threads erzeugen UND starten (-> Zustand runnable):
    if (pthread_create(&faden1, NULL, rauf, NULL) != 0)
    {
        puts("Fehler: pthread_create return code != 0\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&faden2, NULL, runter, NULL) != 0)
    {
        puts("Fehler: pthread_create return code != 0\n");
        exit(EXIT_FAILURE);
    }


    pthread_exit(NULL);		// main-thread beenden, nicht main process!!

    // Die folgenden beiden Anweisungen werden nie ausgefuehrt:
    puts("Main beendet.\n");
    return 0;
}

//-----------------------------------------------------------------------------
// erste thread-function
void* rauf(void* arg)
{
    int i;
    for(i=1; i<=100; i++)
    {
        usleep(10000);	// Mikrosekunden
        printf("rauf: %3d\n", i);
    }
    return NULL;
}

//-----------------------------------------------------------------------------
// zweite thread-function
void* runter(void* arg)
{
    int i;
    for(i=100; i>=1; i--)
    {
        usleep(5000);
        printf("             runter: %3d\n", i);
    }
    return NULL;
}
