/*
 * created on 14/05/23 by Mateusz Glab
 *
 * program takes names from parameters and then create:
 * ->shared memory object
 * ->producer semaphore
 * ->consumer semaphore
 * then program call fork() function to create producer and consumer
 * processes, in child processes function execlp runs consumer and producer program
 * processes are synchronized by semaphores
 *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "smem.h"
#include "sem.h"

#define PATHPRODUCER argv[1]
#define PATHCONSUMER argv[2]
#define PRODUCER argv[3]
#define CONSUMER argv[4]
#define SHAREDM argv[5]
#define SEMPROD argv[6]
#define SEMCONS argv[7]
#define FILEPROD argv[8]
#define FILECONS argv[9]


char *semProd, *semCons, *sharedM;

void cleanUp(void) {
    printf("Clean up\n");
    unlinkSem(semProd);
    unlinkSem(semCons);
    unlinkSM(sharedM);
}

void signalHandler(int signal) {
    printf("SIGINT\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    /* check parameters */
    if (argc != 10) {
        perror("Run program correctly\n");
        printf("Type: ./programName ./producerProgName ./consumerProgName"
               "producerProgName consumerProgName sharedMemName "
               "producerSemaphoreName consumerSemaphoreName"
               "producerFileName consumerFileName");
        exit(EXIT_FAILURE);
    }


    /* register a cleanup function for program termination */
    semProd = SEMPROD;
    semCons = SEMCONS;
    sharedM = SHAREDM;

    if (atexit(cleanUp) != 0) {
        perror("An atexit function error\n");
        exit(1);
    }


    /* create producer semaphore */
    sem_t *semP;
    semP = creatSem(SEMPROD, O_CREAT | O_EXCL, 0666, NBUF);
    printf("\x1b[32mProducer semaphore's adress:\x1b[0m (%p) \x1b[32mvalue:\x1b[0m %d\n", (void *) semP,
           valueSem(semP));

    /* create consumer semaphore */
    sem_t *semC;
    semC = creatSem(SEMCONS, O_CREAT | O_EXCL, 0666, 0);
    printf("\x1b[34mConsumer semaphore's adress:\x1b[0m (%p) \x1b[34mvalue:\x1b[0m %d\n", (void *) semC,
           valueSem(semC));


    /* create shared memory object */
    int sMem = openSM(SHAREDM, O_CREAT | O_EXCL | O_RDWR, 0666);
    /* set memory length */
    smLen(sMem, sizeof(SegmentSM));
    printf("\x1b[33mShared memory's descriptor:\x1b[0m %d \x1b[33msize:\x1b[0m %zu \x1b[33mbytes\x1b[0m\n\n", sMem,
           sizeof(SegmentSM));


    /* handle SIGINT */
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("A signal function error\n");
        exit(EXIT_FAILURE);
    }


    /* create 2 child processes */
    int statLock;
    switch (fork()) {
        case -1:
            perror("First fork error\n");
            exit(EXIT_FAILURE);

            /* child process */
        case 0:
            /* second fork */
            switch (fork()) {
                case -1:
                    perror("Second fork error\n");
                    exit(EXIT_FAILURE);

                    /* consumer's process */
                case 0:
                    if (execlp(PATHCONSUMER, CONSUMER, SHAREDM, SEMPROD, SEMCONS, FILECONS, NULL) == -1) {
                        perror("Cannot run consumer.x\n");
                        exit(EXIT_FAILURE);
                    }
                    break;

                    /* producer's process */
                default:
                    if (execlp(PATHPRODUCER, PRODUCER, SHAREDM, SEMPROD, SEMCONS, FILEPROD, NULL) == -1) {
                        perror("Cannot run producer.x\n");
                        exit(EXIT_FAILURE);
                    }

                    break;
            }
            break;

            /* parent process */
        default:
            break;

    }


    /* wait for child processes */
    if (wait(&statLock) == -1) {
        perror("Wait error\n");
        exit(EXIT_FAILURE);
    }

    /* close shared memory object */
    closeSM(sMem);

    return 0;
}
