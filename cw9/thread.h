#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

/* to create new thread */
pthread_t createPth(void *fun(void *), void *arg);

/* to wait for end of thread */
void joinPth(pthread_t threadID, void **status);

/* for initiate mutex */
void initMutex(pthread_mutex_t *pmutex, const pthread_mutexattr_t *attr);

/* to destroy mutex */
void destroyMutex(pthread_mutex_t *pmutex);

/* to lock mutex */
void lockMutex(pthread_mutex_t *pmutex);

/* to unlock mutex */
void unlockMutex(pthread_mutex_t *pmutex);

/* to set cursor possition */
void hoverOver(unsigned x, unsigned y);

#endif
