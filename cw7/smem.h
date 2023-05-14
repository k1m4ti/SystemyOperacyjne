#ifndef SMEM_H
#define SMEM_H

#include <sys/types.h>
#include <stdbool.h>

#define NELE 20
#define NBUF 5 //number of buffer's elements

typedef struct {
    char buffer[NBUF][NELE];
    int insert, extract;
}SegmentSM;

int openSM(const char *name, int flags, mode_t mode);

void smLen(int fd, off_t length);

void closeSM(int fd);

void unlinkSM(const char *name);

void *mapSM(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

void removeMap(void *addr, size_t length);

bool lastOne(SegmentSM *SM);


#endif
