/*
*   created by Mateusz Glab on 12/05/23
*   
*   the program create (or not) semaphore and then use execlp function  
*   to open incrementer program
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "sem.h"

#define INIT_X argv[1]
#define PROCESSES_NR argv[2]
#define SECTION_NR argv[3]
#define FILE argv[4]
#define SEMAPHORE argv[5]
#define SYNCHRO argv[6]

char *semName;
bool synchro;

void cleanUp(void) {
    if (synchro) unlinkSem(semName);
}

void sigHandler(int signal) {
    if (synchro) {
        printf("SIGINT, unlinking semaphore\n");
        _exit(0);
    }
}

int main(int argc, char *argv[]) {

    /* checking parameters */
    if (argc != 7) {
        perror("Incorrect number of parameters\n");
        printf("To run program correctly type: ./programName initatingProgram "
               "numberOfProcesses numberOfSections textFile semaphoreName synchronization");
        exit(EXIT_FAILURE);
    }

    semName = SEMAPHORE;
    (SYNCHRO[0] == '1') ? (synchro = true) : (synchro = false);

    if (atexit(cleanUp) != 0) {
        perror("Atexit error\n");
        exit(EXIT_FAILURE);
    }

    /* signal handle */
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        perror("Signal error\n");
        exit(EXIT_FAILURE);
    }


    /* create file */
    int file;
    if ((file = open(FILE, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1) {
        perror("Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    /* write '0' to file */
    if (write(file, "0", sizeof("0")) == -1) {
        perror("Cannot write to file\n");
        exit(EXIT_FAILURE);
    }

    /* close file */
    if (close(file)) {
        perror("Close file error\n");
        exit(EXIT_FAILURE);
    }


    /* create semaphore */
    if (synchro) {
        int semVal;
        sem_t *sem;
        createSem(SEMAPHORE, &sem);
        valueSem(sem, &semVal);
        printf("Semaphore's adress: (%p)\nSemaphore's value: %d\n", (void *) sem, semVal);
    }


    /* init pathname */
    char *init;
    if ((init = malloc(strlen(INIT_X) + 3)) == NULL) {
        perror("Malloc error\n");
        exit(EXIT_FAILURE);
    }
    init[0] = '.';
    init[1] = '/';
    strcat(init, INIT_X);


    /* create n-processes */
    int processesNum = atoi(PROCESSES_NR);
    int statLock = 0;
    for (int i = 0; i < processesNum; i++) {
        switch (fork()) {
            case -1:
                perror("Fork error\n");
                exit(EXIT_FAILURE);

            case 0:
                if (execlp(init, INIT_X, SECTION_NR, FILE, SEMAPHORE, SYNCHRO, NULL) == -1) {
                    printf("%s", init);
                    perror("Execlp error\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
        }
    }

    free(init);

    /* waiting for child processes */
    for(int i = 0; i < processesNum; i++){
        if(wait(&statLock) == -1){
            perror("Wait error\n");
            exit(EXIT_FAILURE);
        }
    }


    /* open file again */
    if((file = open(FILE, O_RDONLY, 0666)) == -1){
        perror("Open again exit\n");
        exit(EXIT_FAILURE);
    }

    /* read data from file */
    char buffer[10];
    ssize_t readen = read(file, buffer, sizeof(buffer));

    switch(readen){
        case -1:
            perror("Cannot read data from file\n");
            exit(EXIT_FAILURE);

        case 0:
            perror("File is empty\n");
            exit(EXIT_FAILURE);

        /* checking validity */
        default:
            buffer[readen] = '\0';
            int num = atoi(PROCESSES_NR) * atoi(SECTION_NR);
            if(atoi(buffer) == num)
                printf("\x1b[32mProgram exit succes, number in file %s is equal to %d\x1b[0m\n", buffer, num);
            else
                printf("\x1b[31mProgram exit failure, number in file %s ,isn't equal to %d\x1b[0m\n", buffer, num);
            break;
    }

    /* close file */
    if(close(file)){
        perror("Cannot close file\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
