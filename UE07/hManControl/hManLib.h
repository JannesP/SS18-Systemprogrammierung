//-----------------------------------------------------------------------------
//
// hManLib.h
//
// - Funktionen zum Zeichnen von Hampelmännern
// - die Cursorsteuerung erfolgt mit ANSI-ESCAPE-Sequenzen
//
// - Funktion liesCharVonStdin zum Lesen eines einzelnen Zeichens von stdin
//
//-----------------------------------------------------------------------------

#ifndef HMANLIB_H_INCLUDED
#define HMANLIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <termios.h>

#include <unistd.h>

//------------------------------------------------------------------------------
void zeichneUpDownManAt(int x, int y, int up)
{
    char zeile[20];
    char spalte[20];

    char* topS;
    char* midS;
    char* botS;

    if(up)
    {
        topS ="\\o/";
        midS = " O ";
    }
    else
    {
        topS = " o ";
        midS = "/O\\";
    }

    botS = "/ \\";

    sprintf(zeile, "%d", y);
    sprintf(spalte, "%d", x);
    printf("\033[%s;%sH%s", zeile, spalte, topS);

    sprintf(zeile, "%d", ++y);
    printf("\033[%s;%sH%s", zeile, spalte, midS);

    sprintf(zeile, "%d", ++y);
    printf("\033[%s;%sH%s", zeile, spalte, botS);

    // cursor nach unten links:
    printf("\033[%s;%sH%s", "100", "0", "");

    fflush(stdout);
}

//------------------------------------------------------------------------------
void clearDisplay()
{
    printf("\033[2J");
    fflush(stdout);
}

//------------------------------------------------------------------------------
void initDisplay()
{
    printf("\033[1m");  // bold
    clearDisplay();
}

//------------------------------------------------------------------------------
void machRot()
{
    printf("\033[31m"); // foreground rot
    fflush(stdout);
}

//------------------------------------------------------------------------------
void machGruen()
{
    printf("\033[32m"); // foreground gruen
    fflush(stdout);
}

//-----------------------------------------------------------------------------
// siehe http://stackoverflow.com/questions/10004895/c-reading-from-stdin-as-characters-are-typed
char liesCharVonStdin()
{
    char r[10];
    struct termios old, neu;
    char c;

    tcgetattr(0,&old);

    neu = old;
    neu.c_lflag&=~ICANON;

    tcsetattr(0,TCSANOW,&neu);

    read(0,&c,1);
    r[0] = c;

    // cursor eine Position zurueck:
    printf("\033[1D");
    fflush(stdout);

    return r[0];
}

#endif // HMANLIB_H_INCLUDED
