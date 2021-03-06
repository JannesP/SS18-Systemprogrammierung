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
typedef struct size {
    int left; int top; int width; int height; int r; int g; int b;
} size;

void initSDLFenster();
int getZufallsZahl(int von, int bis);
void* zeichneQuadratAnZufaelligerPositionFromGlobal(void* arg);
void* startUndSync(void* arg);
void createThreads(pthread_t* a_threads);

void joinThreads(pthread_t* a_threads) ;

// ----------------------------GLOBALS-----------------------------------------
SDL_Surface *surface;
size* currSize;
pthread_barrier_t barrier;

// ----------------------------------------------------------------------------
int main()
{
    // Zufallsgenerator initialisieren:
    time_t t;
    srand((unsigned) time(&t));

    // Grafikfenster öffnen:
    initSDLFenster();

    pthread_t t1;
    if (pthread_create(&t1, NULL, startUndSync, NULL) != 0)
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

void* startUndSync(void* arg)
{
    int surfaceWidth = SDL_GetVideoSurface()->w;
    int surfaceHeight = SDL_GetVideoSurface()->h;
    size* sizeBlue = calloc(sizeof(size), 3);
    sizeBlue->width = surfaceWidth;
    sizeBlue->height = surfaceHeight / 3;
    sizeBlue->b = 255;

    size* sizeRed = sizeBlue + 1;
    sizeRed->top = (surfaceHeight / 3) * 2;
    sizeRed->width = surfaceWidth;
    sizeRed->height = surfaceHeight / 3;
    sizeRed->r = 255;

    size* sizeWhite = sizeBlue + 2;
    sizeWhite->top = surfaceHeight / 3;
    sizeWhite->width = surfaceWidth;
    sizeWhite->height = surfaceHeight / 3;
    sizeWhite->r = 255;
    sizeWhite->b = 255;
    sizeWhite->g = 255;

    pthread_t threads[100];
    int i;
    pthread_barrier_init(&barrier, NULL, 101);
    for (i = 45; i > 0; i--)
    {
        createThreads(threads);
        currSize = sizeBlue;
        pthread_barrier_wait(&barrier);
        joinThreads(threads);

        createThreads(threads);
        currSize = sizeWhite;
        pthread_barrier_wait(&barrier);
        joinThreads(threads);

        createThreads(threads);
        currSize = sizeRed;
        pthread_barrier_wait(&barrier);
        joinThreads(threads);
    }
    free(sizeBlue);

    printf("All squares drawn.\n");
}

void createThreads(pthread_t* a_threads) {
    int i;
    for(i = 100; i > 0; i--)
    {
        if (pthread_create(&a_threads[i - 1], NULL, zeichneQuadratAnZufaelligerPositionFromGlobal, NULL) != 0)
        {
            printf("Fehler: pthread_create return code != 0\n");
            exit(EXIT_FAILURE);
        }
    }
}

void joinThreads(pthread_t* a_threads) {
    int n;
    for(n = 100; n > 0; n--) {
        pthread_join((pthread_t) (a_threads[n - 1]), NULL);
    }
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

void* zeichneQuadratAnZufaelligerPositionFromGlobal(void* arg)
{
    printf("thread started and waiting ...\n");
    pthread_barrier_wait(&barrier);
    printf("thread finished waiting ...\n");
    int min = (int)fmin(currSize->height, currSize->width);
    int kantenLaengeQuadrat = min/10;

    int i;
    int anzahlQuadrate = 3;

    for(i = 1; i <= anzahlQuadrate; i++)
    {
        // Verzögerung in Mikrosekunden:
        usleep(1000);

        // Quadrat an zufaelliger Position:
        SDL_Rect r1;
        r1.x = getZufallsZahl(currSize->left, currSize->left + currSize->width - kantenLaengeQuadrat);
        r1.y = getZufallsZahl(currSize->top, currSize->top + currSize->height - kantenLaengeQuadrat);
        r1.w = kantenLaengeQuadrat;
        r1.h = kantenLaengeQuadrat;
        SDL_FillRect(surface, &r1, SDL_MapRGB(surface->format, currSize->r, currSize->g, currSize->b));
        // flip double buffer, d.h. anzeigen:
        if(SDL_Flip(surface) == -1)
        {
            puts("initSDLFenster meldet:  SDL_Flip error\n");
            exit(1);
        }
        printf("thread ran to end\n");
    }

    return NULL;
}