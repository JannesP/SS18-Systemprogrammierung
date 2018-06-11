//------------------------------------------------------------------------------
// showPhilos.c
//
// Installation:
// sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
//
// gcc -Wall showPhilos.c -o showPhilos -lGL -lGLU -lglut -lpthread -lm
//
// Mit CODE::BLOCKS:
//  - Linker Options GL, GLU, glut eintragen unter:
//                                  Project->Build Options->Linker Settings ...
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>

#define pi 3.14159265358979323846

#define anzahlPhilos 5

#define fensterBreite 600
#define fensterHoehe 600

//------------------------------------------------------------------------------
// lokale Kopien der shared memory-arrays, um den krit. Abschnitt
// zu minimieren:
int philoStates[anzahlPhilos];  // 0:denkt, 1:isst
int gabelStates[anzahlPhilos];  // 0:frei,  1:belegt

//------------------------------------------------------------------------------
// Zeiger in shared memory arrays (werden in initSharedMem initialisiert):
int* shmPhiloStates;   // 0:denkt, 1:isst
int* shmGabelStates;   // 0:frei,  1:belegt

//------------------------------------------------------------------------------
void init(void);
void keyboard(unsigned char key, int x, int y);
void display(void);
void machKreis(double x0, double y0, double radius, const GLubyte* farbe);
void machLinie(double x0, double y0, double x1, double y1, int linienBreite, const GLubyte* farbe);
void reshape(int width, int height);
void updateLoop();
void initSharedMem();
void meinSIGINT_Handler();
void registriere_SIGINT();

//------------------------------------------------------------------------------
GLubyte rot[]    = {255, 0, 0};
GLubyte weiss[]  = {255, 255, 255};
GLubyte orange[] = {255, 153, 0};
GLubyte dGrau[]  = {64, 64, 64};
GLubyte hGrau[]  = {176, 176, 176};
GLubyte HGrau[]  = {224, 224, 224};

// Tischradius (Basislänge für die Geometrie):
double rTable = 0.4;

// -----------------------------------------------------------------------------
// POSIX:SEM named-semaphor Name:
char* shmSemaphorName = "/fourtytwo";   // KEIN filename, nur identifier
sem_t* shmSemaphorP;

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    registriere_SIGINT();

    initSharedMem();

    // named-Semaphor erzeugen (ist Pointer):
    shmSemaphorP = sem_open(shmSemaphorName, O_CREAT, 0644, 1); // 1 -> frei
    if(shmSemaphorP == SEM_FAILED)
    {
        printf("errno: %d \n", errno);
        fflush(stdout);
    }

    init();

    glutInit(&argc, argv);

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );

    glutInitWindowPosition(300,150);
    glutInitWindowSize(fensterBreite,fensterHoehe);
    glutCreateWindow("showPhilos");

    // handler registrieren:
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(updateLoop);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}

//------------------------------------------------------------------------------
void init(void)
{
    int i;

    sem_wait(shmSemaphorP);         // P-Operation
        for(i = 0; i < anzahlPhilos; i++)
        {
            shmPhiloStates[i] = 0;     // denkt
            shmGabelStates[i] = 0;     // frei
        }
    sem_post(shmSemaphorP);         // V-Operation
}

//------------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:    // ESC
        meinSIGINT_Handler();   // Strg-C
        exit(0);
    }
}

//------------------------------------------------------------------------------
// Hier wird kein shared-memory verwendet, nur die lokalen Kopien:
void display(void)
{
    int i;

    glClearColor(0.98, 0.98, 0.95, 0.0); // background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Tisch:
    machKreis(0.0, 0.0, rTable, orange);

    // Teller:
    double rPlate  = rTable * 0.25;
    double rpPlate = rTable * 0.7;      // Mittelpunktposition

    // Gabeln (Laenge = Tellerdurchmesser):
    double rpFork0 = rpPlate - rPlate;
    double rpFork1 = rpPlate + rPlate;
    int gabelBreite = 10;

    // Stuehle:
    double rpStuhl = rTable * 1.35;
    double rStuhl  = rTable * 0.3;

    // Philos, denkend:
    double rThink  = rStuhl;
    double ypThink = rpStuhl + 2.5 * rThink;

    // Philos im Denkraum:
    double x0, y0, x1, y1;
    double dxpThink = rThink * 3.0;
    x0 = -2.0 * dxpThink;
    for( i = 0; i < anzahlPhilos; i++)
    {
        if( philoStates[i])
            machKreis(x0, ypThink, rThink, weiss);  // isst -> hier unsichtbar
        else
            machKreis(x0, ypThink, rThink, hGrau);  // denkt -> grau

        x0 = x0 + dxpThink;
    }


    double alpha, deltaAlpha, alphaGabel;
    alpha = 0.0;
    deltaAlpha = 2.0 * pi / anzahlPhilos;

    for( i=0; i< anzahlPhilos; i++)
    {
        // Teller:
        x0 = rpPlate * sin(alpha);
        y0 = rpPlate * cos(alpha);
        machKreis(x0, y0, rPlate, HGrau);

        // Stuehle:
        x0 = rpStuhl * sin(alpha);
        y0 = rpStuhl * cos(alpha);
        if( philoStates[i])
            machKreis(x0, y0, rStuhl, rot);
        else
            machKreis(x0, y0, rStuhl, dGrau);

        // Gabeln:
        alphaGabel = alpha + deltaAlpha / 2.0;
        x0 = rpFork0 * sin(alphaGabel);
        y0 = rpFork0 * cos(alphaGabel);
        x1 = rpFork1 * sin(alphaGabel);
        y1 = rpFork1 * cos(alphaGabel);

        if( gabelStates[i])
            machLinie( x0,  y0,  x1,  y1,  gabelBreite, rot);
        else
            machLinie( x0,  y0,  x1,  y1,  gabelBreite, dGrau);

        alpha = alpha + deltaAlpha;
    }

    glutSwapBuffers();
}

//------------------------------------------------------------------------------
// hier ist die polling-Frequenz des Prozesses definiert:
void updateLoop()
{
    int sleepMs = 100;              // update-Frequenz in Millisekunden
    int i;

    usleep(sleepMs * 1000);

    // shared-memory auslesen:
    sem_wait(shmSemaphorP);                 // P-Operation
        for( i = 0; i < anzahlPhilos; i++)
        {
            philoStates[i] = shmPhiloStates[i];
            gabelStates[i] = shmGabelStates[i];
        }
    sem_post(shmSemaphorP);                 // V-Operation

    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void machKreis(double x0, double y0, double radius, const GLubyte* farbe)
{
    glColor3ubv(farbe);

    int anzahlSegmente = 30;
    int i;

    double alpha, deltaAlpha, x, y;

    alpha = 0.0;
    deltaAlpha = 2.0 * pi / anzahlSegmente;

    glBegin(GL_POLYGON);
    for(i = 1; i <= anzahlSegmente; i++)
    {
        x = x0 + radius * sin(alpha);
        y = y0 + radius * cos(alpha);

        glVertex2d(x, y);

        alpha = alpha + deltaAlpha;
    }
    glEnd();
}

//------------------------------------------------------------------------------
void machLinie(double x0, double y0, double x1, double y1, int linienBreite, const GLubyte* farbe)
{
    glColor3ubv(farbe);
    glLineWidth((GLfloat)linienBreite);

    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();

    glLineWidth(1.0f);    // reset
}

// ------------------------------------------------------------------
void reshape(int width, int height)
{
    glutReshapeWindow( fensterBreite, fensterHoehe);
}

// ------------------------------------------------------------------
void initSharedMem()
{
    char* meinShmFile = "/tmp/philoShm";

    // meinShmFile erzeugen, falls nicht existent:
    open(meinShmFile, O_RDWR | O_CREAT, 0644);  // rw-r--r--

    key_t key;
    int shmId;
    int* shmStartAdr;

    key = ftok(meinShmFile, 42);

    // zwei int-arrays: shmPhiloStates und shmGabelStates:
    size_t shmGroesse = 2 * anzahlPhilos * sizeof(int);
    shmId = shmget(key, shmGroesse, 0644 | IPC_CREAT);  // rw-r--r--

    shmStartAdr = shmat(shmId, NULL, 0); // NULL -> OS wählt Adresse

    // Beide arrays hintereinander:
    shmPhiloStates = (int*) &shmStartAdr[0];   // 0:denkt, 1:isst
    shmGabelStates = (int*) &shmStartAdr[5];   // 0:frei,  1:belegt
}

//-----------------------------------------------------------------------------
void meinSIGINT_Handler()   // Strg-C
{
    // named Semaphor entfernen:
    int erg = sem_unlink(shmSemaphorName);

    if(erg == 0)
        printf("named-semaphor %s entfernt\n", shmSemaphorName);
    else
    {
        printf("named-semaphor entfernen fehlgeschlagen\n");
    }

    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
void registriere_SIGINT()
{
    // Handler meinSIGINT_Handler fuer Strg-C SIGINT registrieren-------:
    struct sigaction sigact01;

    sigact01.sa_handler = meinSIGINT_Handler;
    sigact01.sa_flags = 0;
    sigemptyset(&sigact01.sa_mask);

    if (sigaction(SIGINT, &sigact01, NULL) == -1)    // Registrierung
    {
        perror("FEHLER: sigaction fehlgeschlagen\n");
        exit(1);
    }
}
