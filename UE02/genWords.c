/*
 ============================================================================
 Name        : genWords.c
 ============================================================================
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// ------------------------------------------------------------------
// liefert zufaelligen int-Wert zwischen von und bis einschliesslich.
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

// ------------------------------------------------------------------
void printKleinenZufallsBuchstaben()
{
	printf("%c", getZufallsZahl((int)'a', (int)'z'));
}

// ------------------------------------------------------------------
void printGrossenZufallsBuchstaben()
{
	printf("%c", getZufallsZahl((int)'A', (int)'Z'));
}

// ------------------------------------------------------------------
void printKleinenOderGrossenZufallsBuchstaben()
{
	if(rand() % 2)
		printKleinenZufallsBuchstaben();
	else
		printGrossenZufallsBuchstaben();
}

// ------------------------------------------------------------------
void printZufallsWort(int minLaenge, int maxLaenge)
{
	int laenge = getZufallsZahl(minLaenge, maxLaenge);

	printKleinenOderGrossenZufallsBuchstaben();

	int i;
	for(i = 2; i <= laenge; i++)
		printKleinenZufallsBuchstaben();
}

// ------------------------------------------------------------------
int main(void)
{
	int i;
	time_t t;

	int anzahlWoerter = 100;
	int minWortLaenge100 = 2;
	int maxWortLaenge100 = 50;

	srand((unsigned) time(&t));

	for (i = 1; i <= anzahlWoerter; i++)
	{
		printZufallsWort(minWortLaenge100, maxWortLaenge100);
		printf("\n");
	}

	return 0;
}
