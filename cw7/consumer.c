/*
 * created on 14/05/23 by Mateusz Glab
 *
 * program takes names from parameters and then open:
 * ->shared memory object
 * ->producer semaphore
 * ->consumer semaphore
 * ->destination file
 * then program map a memory, read and transferring data from memory buffer to destination file
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


#include "smem.h"
#include "sem.h"

#define SHAREDM argv[1]
#define SEMPROD argv[2]
#define SEMCONS argv[3]
#define FILECONS argv[4]

int main(int argc, char *argv[]) {

    /* check parameters */
    if (argc != 5) {
        perror("Run consumer program correctly\n");
        printf("Type: ./consumerProgName sharedMemName producerSemaphoreName consumerSemaphoreName "
               "consumerFileName");
        exit(EXIT_FAILURE);
    }

    /* create consumer file */
    int fileDes;
    if ((fileDes = open(FILECONS, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1) {
        perror("Failed to open consumer file\n");
        exit(EXIT_FAILURE);
    }

    /* open producer semaphore */
    sem_t *semP;
    semP = openSem(SEMPROD, O_RDONLY);
    printf("\x1b[34mConsumer opened semaphore's adress:\x1b[0m (%p) \x1b[34mvalue:\x1b[0m %d\n", (void *) semP,
           valueSem(semP));

    /* open consumer semaphore */
    sem_t *semC;
    semC = openSem(SEMCONS, O_RDONLY);
    printf("\x1b[34mConsumer opened semaphore's adress:\x1b[0m (%p) \x1b[34mvalue:\x1b[0m %d\n", (void *) semC,
           valueSem(semC));

    /* open shared memory object */
    int sMem = openSM(SHAREDM, O_RDWR, 0666);
    printf("\x1b[34mConsumer opened shared memory's descriptor:\x1b[0m %d\n\n", sMem);

    /* map memory */
    SegmentSM *SharedMemory;
    SharedMemory = (SegmentSM *) mapSM(NULL, sizeof(SegmentSM), PROT_READ | PROT_WRITE, MAP_SHARED, sMem, 0);

    /* write to file loop */
    SharedMemory->extract = 0;

    srand(time(NULL));

    while (1) {
        /* wait random time */
        sleep(rand() % 5);

        /* wait consumer semaphore */
        waitSem(semC);
        printf("\x1b[34mConsumer semaphore's value before critical section:\x1b[0m %d\n", valueSem(semC));

        if (lastOne(SharedMemory)) {
            /* write data to file */
            if (write(fileDes, SharedMemory->buffer[SharedMemory->extract],
                      strlen(SharedMemory->buffer[SharedMemory->extract])) == -1) {
                printf("write error\n");
                exit(EXIT_FAILURE);
            }

            printf("\x1b[31mEOF consumer\x1b[0m %s\n", SharedMemory->buffer[SharedMemory->extract]);

            /* post producer semaphore */
            postSem(semP);
            printf("\x1b[34mProducer semaphore's value after critical section:\x1b[0m %d\n\n", valueSem(semP));
            break;
        }

        /* write data to file */
        if (write(fileDes, SharedMemory->buffer[SharedMemory->extract], NELE) == -1) {
            printf("write error\n");
            exit(EXIT_FAILURE);
        }

        /* dynamic memory buffer */
        char *termOut = (char *) malloc(25 + strlen(SharedMemory->buffer[SharedMemory->extract]));
        if (termOut == NULL) {
            perror("Malloc error\n");
            exit(EXIT_FAILURE);
        }

        /* write data to terminal */
        sprintf(termOut, "%s%s%c", "\x1b[31mDownloading... \x1b[0m", SharedMemory->buffer[SharedMemory->extract], '\n');
        printf("\x1b[35mBuffer extract index:\x1b[0m %d\n", SharedMemory->extract);
        if (write(STDOUT_FILENO, termOut, strlen(termOut)) == -1) {
            perror("Write to terminal is impossible, write error\n");
            exit(EXIT_FAILURE);
        }

        free(termOut);

        /* change buffer index */
        SharedMemory->extract = (SharedMemory->extract + 1) % NBUF;

        /* post producer semaphore */
        postSem(semP);
        printf("\x1b[34mProducer semaphore's value after critical section:\x1b[0m %d\n\n", valueSem(semP));

    }

    /* close file */
    if (close(fileDes) == -1) {
        perror("Failed to close consumer file\n");
        exit(EXIT_FAILURE);
    }

    removeMap(SharedMemory, sizeof(SegmentSM));

    closeSem(semP);
    closeSem(semC);

    return 0;
}
