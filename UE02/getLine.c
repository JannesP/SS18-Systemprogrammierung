#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// ------------------------------------------------------------------
void meldungUndExit(char* text)
{
	printf("%s\n", text);
	printf("Prozess wird beendet");
	exit(EXIT_FAILURE);
}


// ------------------------------------------------------------------
// getLine liest eine Zeile (max. 500 Zeichen) von stdin (ohne LF).
// Diese Zeichen plus ein \0 werden in einen entsprechend  neu
// allokierten heap-buffer kopiert.
// Zurueckgegeben wird der Zeiger auf den heap-buffer oder NULL, falls
// kein Zeichen gelesen wurde.
// Der buffer wird NICHT deallokiert.
// ------------------------------------------------------------------
char* getLine()
{
	const int laengeBuffer = 501;	// 500 + \0
	char tempBuffer[laengeBuffer];

	int indexLetztesZeichen = laengeBuffer - 2; // wg. \0
	int i = 0;
	int tempChar;	// getchar liefert int

	// Zeichen in tempBuffer lesen:
	while( i <= indexLetztesZeichen && (tempChar = getchar()) != '\n' && tempChar != EOF )
	{
		tempBuffer[i] = tempChar;
		i++;
	}
	tempBuffer[i] = '\0';

	if( i == 0 )	// kein Zeichen gelesen (leere Zeile oder nur EOF)
		return NULL;

	// Zeile in den heap kopieren:
	char* pHeapBuffer;

	pHeapBuffer = malloc(i + 1);	// Anzahl ist Index + 1

	if(pHeapBuffer == NULL)
		meldungUndExit("getLine meldet: \"malloc fehlgeschlagen\"");

	strcpy(pHeapBuffer, tempBuffer);

	return pHeapBuffer;
}

