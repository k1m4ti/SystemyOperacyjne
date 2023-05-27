#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

/* to create new thread */
pthread_t createPth(void *fun(void *), void *arg);

/* to wait for end of thread */
void joinPth(pthread_t threadID, void **status);

/* to set cursor possition */
void hoverOver(unsigned x, unsigned y);

#endif
