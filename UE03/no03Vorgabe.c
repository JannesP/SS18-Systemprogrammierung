//-----------------------------------------------------------------------------
// no03Vorgabe.c
//
// Ein thread zeichnet 5000 blaue Quadrate in ein Xlib/SDL-Fenster.
//
// Programm erfordert SDL 1.2:
// 	- sudo apt-get install libsdl1.2-dev
//
// zu SDL 1.2: http://www.libsdl.org/release/SDL-1.2.15/docs/html/index.html
//
// gcc no03Vorgabe.c -o no03Vorgabe -Wall -lpthread -lm -lSDL -lX11
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <SDL/SDL.h>

#include <X11/Xlib.h>	//  wg. XInitThreads()

// ----------------------------PROTOTYPES--------------------------------------
void initSDLFenster();
int getZufallsZahl(int von, int bis);
void* zeichneBlaueQuadrateAnZufaelligerPosition();

// ----------------------------GLOBALS-----------------------------------------
SDL_Surface *surface;


// ----------------------------------------------------------------------------
int main()
{
    // Zufallsgenerator initialisieren:
    time_t t;
    srand((unsigned) time(&t));

    // Grafikfenster öffnen:
    initSDLFenster();

    // thread starten:
    pthread_t faden1;
    if (pthread_create(&faden1, NULL, zeichneBlaueQuadrateAnZufaelligerPosition, NULL) != 0)
    {
        puts("Fehler: pthread_create return code != 0\n");
        exit(EXIT_FAILURE);
    }

    // main event-loop:
    int quit = 0;
    SDL_Event event;
    while(!quit)
    {
        // auf Tastendruck lauern, evtl. loop beenden:
        while(SDL_PollEvent(&event))
            if(event.type==SDL_KEYDOWN)
                quit=1;
    }

    // aufraeumen und Schluss:
    SDL_Quit();
    return 0;
}

// ----------------------------------------------------------------------------
// SDL-Fenster initialisieren und öffnen
void initSDLFenster()
{
    // Xlib fuer multithreading initialisieren:
    XInitThreads();

    const int fensterBreite = 600;
    const int fensterHoehe = 300;
    const int farbTiefe = 32;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("initSDLFenster meldet:  %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption( "no03Vorgabe | Tastendruck -> Ende", NULL );

    atexit(SDL_Quit);

    surface = SDL_SetVideoMode(fensterBreite, fensterHoehe, farbTiefe, SDL_DOUBLEBUF);
    if (surface == NULL)
    {
        printf("initSDLFenster meldet: %s\n", SDL_GetError());
        exit(1);
    }

    // Hintergrund grau fuellen:
    SDL_Rect hintergrund;
    hintergrund.x = 0;
    hintergrund.y = 0;
    hintergrund.w = fensterBreite;
    hintergrund.h = fensterHoehe;

    SDL_FillRect(surface, &hintergrund, SDL_MapRGB(surface->format, 102, 102, 102));

    // flip double buffer, d.h. anzeigen:
    if(SDL_Flip(surface) == -1)
    {
        puts("initSDLFenster meldet:  SDL_Flip error\n");
        exit(1);
    }
}

// ------------------------------------------------------------------
// Liefert zufaelligen int-Wert zwischen von und bis einschliesslich:
int getZufallsZahl(int von, int bis)
{
    if(von >= bis || bis > RAND_MAX)
    {
        puts("getZufallsZahl meldet: von >= bis || bis > RAND_MAX");
        exit(EXIT_FAILURE);
    }

    return rand() % (bis - von + 1) + von;
}

// ----------------------------------------------------------------------------
// thread-function
void* zeichneBlaueQuadrateAnZufaelligerPosition()
{
    int aktFensterBreite = SDL_GetVideoSurface()->w;
    int aktFensterHoehe  = SDL_GetVideoSurface()->h;
    int min = (int)fmin(aktFensterBreite, aktFensterHoehe);
    int kantenLaengeQuadrat = min/50;

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 255;

    int i;
    int anzahlQuadrate = 5000;

    for(i = 1; i <= anzahlQuadrate; i++)
    {
        // Verzögerung in Mikrosekunden:
        usleep(1000);

        // Quadrat an zufaelliger Position:
        SDL_Rect r1;
        r1.x = getZufallsZahl(0, aktFensterBreite - kantenLaengeQuadrat);
        r1.y = getZufallsZahl(0, aktFensterHoehe  - kantenLaengeQuadrat);
        r1.w = kantenLaengeQuadrat;
        r1.h = kantenLaengeQuadrat;
        SDL_FillRect(surface, &r1, SDL_MapRGB(surface->format, r, g, b));

        // flip double buffer, d.h. anzeigen:
        if(SDL_Flip(surface) == -1)
        {
            puts("initSDLFenster meldet:  SDL_Flip error\n");
            exit(1);
        }
    }

    printf("%d Quadrate gezeichnet, beliebige Taste -> ENDE\n", anzahlQuadrate);

    return NULL;
}

