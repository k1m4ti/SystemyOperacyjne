#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pthread_t createPth(void *fun(void *), void *arg) {
    pthread_t tID;
    int status = pthread_create(&tID, NULL, fun, arg);
    if (status == 0) return tID;
    switch (status) {
        case EAGAIN:
            perror("Failed to create pthread\nSystem limit of pthread or processes over flow\n");
            exit(EXIT_FAILURE);

        case ENOMEM:
            perror("Failed to create pthread\nOut of memory to create new pthread\n");
            exit(EXIT_FAILURE);

        case EINVAL:
            perror("Failed to create pthread\nIncorrect value of argument\n");
            exit(EXIT_FAILURE);
    }
    if (status != 0) {
        perror("Failed to create pthread\n");
        exit(EXIT_FAILURE);
    }

    return tID;
}

void joinPth(pthread_t threadID, void **status) {
    if (pthread_join(threadID, status) == -1) {
        perror("Failed to join ");
        exit(EXIT_FAILURE);
    }
}

void initMutex(pthread_mutex_t *pmutex, const pthread_mutexattr_t *attr) {
    int status = pthread_mutex_init(pmutex, attr);
    if (status == EINVAL) {
        perror("Failed to init mutex\nInvalid argument\n");
        exit(EXIT_FAILURE);
    } else if (status != 0) {
        perror("Failed to init mutex\n");
        exit(EXIT_FAILURE);
    }
}

void destroyMutex(pthread_mutex_t *pmutex) {
    int status = pthread_mutex_destroy(pmutex);
    if (status == EBUSY) {
        perror("Failed to destroy mutex\nMutex doesn't exists\n");
        exit(EXIT_FAILURE);
    } else if (status != 0) {
        perror("Failed to destroy mutex\n");
        exit(EXIT_FAILURE);
    }
}

void lockMutex(pthread_mutex_t *pmutex) {
    if (pthread_mutex_lock(pmutex) != 0) {
        perror("Failed to lock mutex\n");
        exit(EXIT_FAILURE);
    }
}

void unlockMutex(pthread_mutex_t *pmutex) {
    if (pthread_mutex_unlock(pmutex) != 0) {
        perror("Failed to unlock mutex\n");
        exit(EXIT_FAILURE);
    }
}

void hoverOver(unsigned x, unsigned y) {
    printf("\033[%d;%dH\033[2K", y, x);
}



