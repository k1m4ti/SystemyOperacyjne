#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#define NELE 20
#define NBUF 5 //number of buffer's elements

typedef struct {
    char buffer[NBUF][NELE];
    int insert, extract;
} SegmentSM;

int openSM(const char *name, int flags, mode_t mode) {
    int fd;
    if ((fd = shm_open(name, flags, mode)) == -1) {
        perror("Shared memory open error\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

/* set a length for memory object */
void smLen(int fd, off_t length) {
    if (ftruncate(fd, length) == -1) {
        perror("Ftruncate error\n");
        exit(EXIT_FAILURE);
    }
}

void closeSM(int fd) {
    if (close(fd) == -1) {
        perror("Failed to close shared memory object\n");
        exit(EXIT_FAILURE);
    }
}

void unlinkSM(const char *name) {
    printf("Unlinking shared memory object: %s\n", name);
    if (shm_unlink(name) == -1) {
        perror("Failed to remove shared memory object\n");
        exit(EXIT_FAILURE);
    }
}

void *mapSM(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void *mappedAddr = mmap(addr, length, prot, flags, fd, offset);
    if (mappedAddr == MAP_FAILED) {
        perror("Failed to map memory\n");
        exit(EXIT_FAILURE);
    }
    return mappedAddr;
}

void removeMap(void *addr, size_t length) {
    if (munmap(addr, length) == -1) {
        perror("Munmap exit\n");
        exit(EXIT_FAILURE);
    }
}

bool lastOne(SegmentSM *SM) {
    for (int i = 0; i < NELE; i++) {
        if (SM->buffer[SM->extract][i] == '\0') {
            return true;
        }
    }
    return false;
}
