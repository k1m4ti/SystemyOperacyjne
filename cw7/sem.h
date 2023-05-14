/*
*   created by Mateusz Glab on 12/05/23
*/

#ifndef SEM_H
#define SEM_H

#include <semaphore.h>
#include <sys/types.h>

sem_t *creatSem(const char *name, int flags, mode_t mode, unsigned int value);

sem_t *openSem(const char *name, int flags);

void closeSem(sem_t *sem);

void unlinkSem(const char *name);

void postSem(sem_t *sem);

void waitSem(sem_t *sem);

int valueSem(sem_t *sem);

#endif
