#ifndef SCHLAFZUFAELLIG_H_INCLUDED
#define SCHLAFZUFAELLIG_H_INCLUDED

#include <stdlib.h>
#include <time.h>


//------------------------------------------------------------------------------
void initSchlafZufaellig()
{
    srand(time(NULL));
}

//------------------------------------------------------------------------------
// Damit jeder Prozess eine individuelle Zahlenfolge abruft, kann der
// Zufallsgenerator z.B. so initialisiert werden:
//
//             initSchlafZufaellig_2((long)getpid());
//
void initSchlafZufaellig_2(long n)
{
    srand(time(NULL) + n);
}

// ------------------------------------------------------------------
// Liefert zufaelligen int-Wert zwischen von und bis einschliesslich.
// ------------------------------------------------------------------
int getZufallsZahl(int von, int bis)
{
	if(von >= bis || bis > RAND_MAX)
	{
		puts("getZufallsZahl meldet: von >= bis || bis > RAND_MAX");
		exit(EXIT_FAILURE);
	}

	return rand() % (bis - von + 1) + von;
}

//------------------------------------------------------------------------------
void schlafZufaellig(int vonMilliSekunden, int bisMilliSekunden)
{
    if(vonMilliSekunden > bisMilliSekunden)
    {
        printf("Fehler in schlafZufaellig:vonMilliSekunden > bisMilliSekunden\n");
    }

    long zufMilliDelay = getZufallsZahl(vonMilliSekunden, bisMilliSekunden);
    long millis = zufMilliDelay % 1000;
    long sek = (zufMilliDelay  - millis) / 1000;

    struct timespec delay;

    delay.tv_sec = sek;
    delay.tv_nsec = millis * 1000000;

    nanosleep(&delay, NULL);
}


#endif // SCHLAFZUFAELLIG_H_INCLUDED
