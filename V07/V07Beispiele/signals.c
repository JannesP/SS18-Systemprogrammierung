//-----------------------------------------------------------------------------
// signals.c
//
// - Ein child-Prozess sendet einem anderen child ein SIGUSR1 Signal
//
// - main wartet exklusiv auf SIGINT (Strg-C), um dann den beiden childs
//   ein Terminierungssignal zu schicken (SIGTERM).
//
// gcc signals.c -o signals -Wall
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>
#include <sys/wait.h>

//-----------------------------------------------------------------------------
// Dieser handler wird von beiden childs verwendet. Es handelt sich jedoch nicht
// um einen kritischen Bereich, da jeder Prozess seine eigene Instanz hat:
//    -> zu Beginn wird zweimal der Wert 1 ausgegeben.
//
void meinSIGUSR1_Handler()
{
    static int count = 0;   // kein Mutex nötig, da nicht global für Prozesse

    printf("PID: %d, eingetroffene Signale: %d\n", getpid(), ++count);
    fflush(stdout);
}

//-----------------------------------------------------------------------------
int main()
{
    printf("main-PID: %d (mit Strg-C beenden)\n", getpid());
    fflush(stdout);


    // Handler meinSIGUSR1_Handler fuer user-Signal SIGUSR1 registrieren-------:
    struct sigaction sigact01;

    sigact01.sa_handler = meinSIGUSR1_Handler;
    sigact01.sa_flags = 0;
    sigemptyset(&sigact01.sa_mask);  // Initialisierung OHNE Standard-Signale

    if (sigaction(SIGUSR1, &sigact01, NULL) == -1)    // Registrierung
    {
        perror("FEHLER: sigaction fehlgeschlagen\n");
        exit(1);
    }


    // Zwei childs erzeugen:
    int childPid1, childPid2;

    // .........................................................................
    childPid1 = fork();
    if(childPid1 == 0)  // child1
    {
        while(1)
            pause();    // auf beliebiges Signal warten, dann handler ausführen
    }

    // .........................................................................
    childPid2 = fork();
    if(childPid2 == 0)  // child2
    {
        pause();        // zu Beginn erst auf Signal warten

        while(1)
        {
            sleep(1);
            kill(childPid1, SIGUSR1);   // Signal an Geschwisterprozess
        }
    }

    // .........................................................................
    // forks abwarten:
    sleep(1);

    // child2 aufwecken, d.h. aus erster 'pause()' holen:
    kill(childPid2, SIGUSR1);

    // Nur auf Signal SIGINT (Strg-C) warten
    // (ohne einen eigenen handler zu registrieren):
    sigset_t sigMaske;
    sigfillset(&sigMaske);  // Initialisierung MIT Standard-Signalen
    sigdelset(&sigMaske, SIGINT);

    sigsuspend(&sigMaske);  // dieser Prozess blockiert jetzt

    // Signal 'terminieren' an childs senden:
    kill(childPid1, SIGTERM);
    kill(childPid2, SIGTERM);

    // auf Terminierung beider childs warten:
    waitpid(childPid1, NULL, 0);
    waitpid(childPid2, NULL, 0);

    return 0;
}
