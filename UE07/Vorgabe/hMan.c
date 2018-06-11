//-----------------------------------------------------------------------------
//
// hMan.c     (Demoprogramm zur hManLib.h)
//
//
// gcc hMan.c -o hMan -Wall
//
//-----------------------------------------------------------------------------

#include "hManLib.h"


int main()
{
    initDisplay();

    while(1)
    {
        usleep(500 * 1000);

        clearDisplay();
        machRot();
        zeichneUpDownManAt( 5, 5, 1);

        usleep(500 * 1000);

        clearDisplay();
        machGruen();
        zeichneUpDownManAt(35, 10, 0);
    }

    return 0;
}
