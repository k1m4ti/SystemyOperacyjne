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


void hoverOver(unsigned x, unsigned y) {
    printf("\033[%d;%dH\033[2K", y, x);
}
