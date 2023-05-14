/*
 * created on 14/05/23 by Mateusz Glab
 *
 * program takes names from parameters and then open:
 * ->shared memory object
 * ->producer semaphore
 * ->consumer semaphore
 * ->source file
 * then program map a memory, read and transferring data from file to shared memory buffer
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#include "smem.h"
#include "sem.h"

#define SHAREDM argv[1]
#define SEMPROD argv[2]
#define SEMCONS argv[3]
#define FILEPROD argv[4]


int main(int argc, char *argv[]) {

    int statLock;

    /* check parameters */
    if (argc != 5) {
        perror("Run producer program correctly\n");
        printf("Type: ./producerProgName sharedMemName producerSemaphoreName consumerSemaphoreName "
               "producerFileName");
        exit(EXIT_FAILURE);
    }


    /* open producer file */
    int fileDes;
    if ((fileDes = open(FILEPROD, O_RDONLY, 0666)) == -1) {
        perror("Failed to open producer file\n");
        exit(EXIT_FAILURE);
    }


    /* open producer semaphore */
    sem_t *semP;
    semP = openSem(SEMPROD, O_RDONLY);
    printf("\x1b[32mProducer opened semaphore's adress:\x1b[0m (%p) \x1b[32mvalue:\x1b[0m %d\n", (void *) semP,
           valueSem(semP));

    /* open consumer semaphore */
    sem_t *semC;
    semC = openSem(SEMCONS, O_RDONLY);
    printf("\x1b[32mProducer opened semaphore's adress:\x1b[0m (%p) \x1b[32mvalue:\x1b[0m %d\n", (void *) semC,
           valueSem(semC));

    /* open shared memory object */
    int sMem = openSM(SHAREDM, O_RDWR, 0666);
    printf("\x1b[32mProducer opened shared memory's descriptor:\x1b[0m %d\n\n", sMem);

    /* map memory */
    SegmentSM *SharedMemory;
    SharedMemory = (SegmentSM *) mapSM(NULL, sizeof(SegmentSM), PROT_READ | PROT_WRITE, MAP_SHARED, sMem, 0);

    /* read from file loop */
    SharedMemory->insert = 0;

    srand(time(NULL));
    sleep(1);
    while (1) {

        /* wait random time */
        sleep(rand() % 5);

        /* wait producer semaphore */
        waitSem(semP);
        printf("\x1b[32mProducer semaphore's value before critical section:\x1b[0m %d\n", valueSem(semP));

        /* read data from file */
        ssize_t readen;
        if ((readen = read(fileDes, SharedMemory->buffer[SharedMemory->insert], NELE)) == -1) {
            perror("Failed to read data from file\n");
            exit(EXIT_FAILURE);
        }

        /* break a loop*/
        if (readen != NELE) {
            SharedMemory->buffer[SharedMemory->insert][readen] = '\0';
            // printf("Uploading... %s\n", SharedMemory->buffer[SharedMemory->insert]);
            printf("\x1b[31mEOF producer\x1b[0m\n");

            /* post consumer semaphore last time */
            postSem(semC);
            printf("\x1b[32mConsumer semaphore's value after critical section:\x1b[0m %d\n\n", valueSem(semC));
            break;
        }

        /* dynamic memory buffer */
        char *termOut = (char *) malloc(23 + strlen(SharedMemory->buffer[SharedMemory->insert]));
        if (termOut == NULL) {
            perror("Malloc error\n");
            exit(EXIT_FAILURE);
        }

        /* write data to terminal */
        sprintf(termOut, "%s%s%c", "\x1b[31mUploading... \x1b[0m", SharedMemory->buffer[SharedMemory->insert], '\n');
        printf("\x1b[35mBuffer insert index:\x1b[0m %d\n", SharedMemory->insert);
        if (write(STDOUT_FILENO, termOut, strlen(termOut)) == -1) {
            perror("Write to terminal is impossible, write error\n");
            exit(EXIT_FAILURE);
        }

        free(termOut);

        /* change index*/
        SharedMemory->insert = (SharedMemory->insert + 1) % NBUF;

        /* post consumer semaphore */
        postSem(semC);
        printf("\x1b[32mConsumer semaphore's value after critical section:\x1b[0m %d\n\n", valueSem(semC));

    }

    /* wait for consumer processes */
    if (wait(&statLock) == -1) {
        perror("Wait error\n");
        exit(EXIT_FAILURE);
    }

    /* close file */
    if (close(fileDes) == -1) {
        perror("Failed to close producer file\n");
        exit(EXIT_FAILURE);
    }

    removeMap(SharedMemory, sizeof(SegmentSM));

    closeSem(semP);
    closeSem(semC);

    return 0;
}
