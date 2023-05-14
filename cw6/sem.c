/*
*   created by Mateusz Glab on 12/05/23
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void createSem(const char *name, sem_t **semAdress) {
    if ((*semAdress = sem_open(name, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        perror("Failed to create semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void openSem(const char *name, sem_t **semAdress) {
    if ((*semAdress = sem_open(name, O_RDONLY)) == SEM_FAILED) {
        perror("Cannot to open semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void closeSem(sem_t *sem) {
    if (sem_close(sem) == -1) {
        perror("Semaphore close error\n");
        exit(EXIT_FAILURE);
    }
}

void unlinkSem(const char *name) {
    printf("Unlinking semaphore: %s\n", name);
    if (sem_unlink(name) == -1) {
        perror("Unlink error\n");
        exit(EXIT_FAILURE);
    }
}

void postSem(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Failed to post semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void waitSem(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Semaphore wait error\n");
        exit(EXIT_FAILURE);
    }
}

void valueSem(sem_t *sem, int *sval) {
    if (sem_getvalue(sem, sval) == -1) {
        perror("Semaphore get value error\n");
        exit(EXIT_FAILURE);
    }
}

