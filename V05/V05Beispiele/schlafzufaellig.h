#ifndef SCHLAFZUFAELLIG_H_INCLUDED
#define SCHLAFZUFAELLIG_H_INCLUDED

#include <stdlib.h>


//------------------------------------------------------------------------------
void initSchlafZufaellig()
{
    srand(time(NULL));
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
    if(vonMilliSekunden > 1000 || bisMilliSekunden > 1000)
    {
        printf("Fehler in schlafZufaellig: Parameterwert  > 1000ms nicht zulaessig");
    }

    long zufNanoDelay = getZufallsZahl(vonMilliSekunden, bisMilliSekunden) * 1000 * 1000;

    struct timespec delay;

    delay.tv_sec = 0;
    delay.tv_nsec = zufNanoDelay;

    nanosleep(&delay, NULL);
}


#endif // SCHLAFZUFAELLIG_H_INCLUDED
