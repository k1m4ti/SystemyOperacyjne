/*
*   created by Mateusz Glab on 12/05/23
*/

#ifndef SEM_H
#define SEM_H

#include "semaphore.h"

void createSem(const char *name, sem_t **semAdress);

void openSem(const char *name, sem_t **semAdress);

void closeSem(sem_t *sem);

void unlinkSem(const char *name);

void postSem(sem_t *sem);

void waitSem(sem_t *sem);

void valueSem(sem_t *sem, int *sval);

#endif
