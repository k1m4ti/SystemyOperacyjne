/*
    Created by Mateusz Glab 27/05/23 
    Program creates new threads and then use mutex to synchronization 
    and increase number  
*/

#define _REENTRANT

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "thread.h"

#define THREADS argv[1]
#define SECTIONS argv[2]

typedef struct Arg {
    int nr;
    int sectionsNum;
} Arg;

pthread_mutex_t myMutex;
pthread_t *threads;
int number;
int threadsNum;


void *fun(void *arg);

int main(int argc, char **argv) {
    /* check amount of parameters */
    if (argc != 3) {
        perror("Incorrect number of parameters\n");
        printf("To run program correctly type: ./ProgName NumberOfThreads NumberOfSections\n");
        exit(EXIT_FAILURE);
    }

    threadsNum = atoi(THREADS);
    int sectionsNum = atoi(SECTIONS);

    /* allocate memory for arg struct */
    Arg *arg = (Arg *) malloc(sizeof(Arg) * threadsNum);
    if (arg == NULL) {
        perror("Failed to allocate memory for arg struct\n");
        exit(EXIT_FAILURE);
    }

    /* create vector of threads */
    threads = (pthread_t *) malloc(sizeof(pthread_t) * threadsNum);
    if (threads == NULL) {
        perror("Malloc error\nCannot allocate memory for threads\n");
        exit(EXIT_FAILURE);
    }

    /* initation mutex */
    initMutex(&myMutex, NULL);
    printf("Mutex's adress: %p\n", (void *) &myMutex);

    /* create threads loop */
    for (int i = 0; i < threadsNum; i++) {
        arg[i].nr = i;
        arg[i].sectionsNum = sectionsNum;
        threads[i] = createPth(fun, (void *) &arg[i]);
    }

    /* wait for threads */
    for (int i = 0; i < threadsNum; i++)
        joinPth(threads[i], NULL);

    /* remove mutex */
    destroyMutex(&myMutex);

    /* free allocated memory */
    free(threads);
    free(arg);

    hoverOver(0, 4 + 2 * threadsNum);

    if (number == threadsNum * sectionsNum)
        printf("Synchronization was succesfull\n");
    else
        printf("Synchronization wasn't succesfull\n");

    return 0;
}

/* thread function */
void *fun(void *arg) {
    Arg *ptr = (Arg *) arg;

    printf("Thread nr: %d ID: %p\n", ptr->nr, (void *) threads[ptr->nr]);

    srand(time(NULL));
    sleep(rand() % 3);

    /* sections loop */
    for (int i = 0, count; i < ptr->sectionsNum; i++) {

        hoverOver(0, 4 + threadsNum + ptr->nr);
        printf("\033[32mThread nr: %d, PrivateSession: %d, Num: %d \033[0m\n", ptr->nr, i, number);

        sleep(rand() % 3);

        /* critical section */
        lockMutex(&myMutex);
        hoverOver(25, 4 + threadsNum + ptr->nr);
        printf("\033[35mThread nr: %d, CriticalSession: %d, Num: %d \033[0m\n", ptr->nr, i, number);

        count = number;
        count++;
        sleep(2);
        number = count;

        /* private section */
        unlockMutex(&myMutex);
        hoverOver(0, 4 + threadsNum + ptr->nr);
        printf("\033[32mThread nr: %d, PrivateSession: %d, Num: %d \033[0m\n", ptr->nr, i, number);

        sleep(rand() % 3);

    }
    return NULL;
}
