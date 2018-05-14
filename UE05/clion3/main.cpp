#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <climits>

sem_t sem1, sem2, sem3, sem4, semFinish, semLockPrint;

void* Fischer(void* arg) {
    sem_wait(&sem1);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("Fischer ");
    sem_post(&semLockPrint);
    sem_post(&sem2);
}

void* Fritz(void* arg) {
    sem_wait(&sem1);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("Fritz ");
    sem_post(&semLockPrint);
    sem_post(&sem2);
}

void* fischt(void* arg) {
    sem_wait(&sem2);
    sem_wait(&sem2);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("fischt ");
    sem_post(&semLockPrint);
    sem_post(&sem3);
    sem_post(&sem3);
    sem_post(&sem3);
}

void* fesche(void* arg) {
    sem_wait(&sem3);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("fesche ");
    sem_post(&semLockPrint);
    sem_post(&sem4);
}

void* frische(void* arg) {
    sem_wait(&sem3);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("frische ");
    sem_post(&semLockPrint);
    sem_post(&sem4);
}

void* fette(void* arg) {
    sem_wait(&sem3);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("fette ");
    sem_post(&semLockPrint);
    sem_post(&sem4);
}

void* Fische(void* arg) {
    sem_wait(&sem4);
    sem_wait(&sem4);
    sem_wait(&sem4);
    uint time = (uint)(((double)random() / INT_MAX) * 100 * 1000);
    usleep(time);
    sem_wait(&semLockPrint);
    printf("Fische ");
    sem_post(&semLockPrint);
    sem_post(&semFinish);
}

int main() {
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);
    sem_init(&sem4, 0, 0);
    sem_init(&semFinish, 0, 0);
    sem_init(&semLockPrint, 0, 1);

    pthread_t t1, t2, t3, t4, t5, t6, t7;

    srand((uint)time(NULL));

    pthread_create(&t1, NULL, Fischer, NULL);
    pthread_create(&t2, NULL, Fritz, NULL);
    pthread_create(&t3, NULL, fischt, NULL);
    pthread_create(&t4, NULL, fesche, NULL);
    pthread_create(&t5, NULL, frische, NULL);
    pthread_create(&t6, NULL, fette, NULL);
    pthread_create(&t7, NULL, Fische, NULL);

    sem_post(&sem1);
    sem_post(&sem1);

    sem_wait(&semFinish);
    return 0;
}