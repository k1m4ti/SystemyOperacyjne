/*
*   created on 12/05/23 by Mateusz Glab
*
*   the program is using semaphores to synchronize processes
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


#include "sem.h"

#define SECTION_NR argv[1]
#define FILE argv[2]
#define SEMAPHORE argv[3]
#define SYNCHRO argv[4]

int main(int argc, char* argv[]){

    /* checking parameters */
    if (argc != 5) {
        perror("Incorrect number of parameters\n");
        printf("To run program correctly type: ./programName "
               "numberOfSections textFile semaphoreName synchronization");
        exit(EXIT_FAILURE);
    }

    /* if synchronization on */
    bool synchro;
    (SYNCHRO[0] == '1') ? (synchro = true) : (synchro = false);

    /* open semaphore */
    sem_t *sem;
    if(synchro){
       openSem(SEMAPHORE, &sem);
    }

    /* get number of critical sections */
    int sections = atoi(SECTION_NR);

    for(int i = 0; i < sections; i++){
        /* wait random amount of time */
        srand(time(NULL));
        sleep(rand() % 5);

        /* semaphore passeren */
        int semVal;
        if(synchro){
            valueSem(sem, &semVal);
            waitSem(sem);
            printf("PID = %d, semaphore's value before critical section = %d\n", getpid(), semVal);
        }

        /* critical section */
        int file;
        if((file = open(FILE, O_RDONLY))== -1){
            perror("Failed to open file to read only\n");
            exit(EXIT_FAILURE);
        }

        char buffer[20];
        ssize_t readen = read(file, buffer, sizeof(buffer));

        bool a = false;
        bool *ptr;
        ptr = &a;

        switch(readen){
            case -1:
                perror("Failed to read file\n");
                exit(EXIT_FAILURE);

            case 0:
                *ptr = true;
                break;

            default:
                buffer[readen] = '\0';
                if(synchro) {
                    valueSem(sem, &semVal);
                    printf("PID = %d, semaphore's value = %d, read nr from file = %s, section nr = %d\n", getpid(),
                           semVal, buffer, i);
                }
        }

        if(a) break;    //leave for loop

        /* close file */
        if(close(file)){
            perror("Cannot close file\n");
            exit(EXIT_FAILURE);
        }

        /* increase number by one */
        int number = atoi(buffer);
        number++;

        /* write increased number to buffer */
        sprintf(buffer, "%d", number);

        /* wait random amount of time again */
        sleep(rand() % 5);

        /* open file to write */
        if((file = open(FILE, O_TRUNC | O_WRONLY | O_CREAT, 0666)) == -1){
            printf("Failed to open file to write data\n");
            exit(EXIT_FAILURE);
        }

        /* write data to file */
        if(write(file, buffer, strlen(buffer)) == -1){
            perror("Write to file error\n");
            exit(EXIT_FAILURE);
        }

        /* close file */
        if(close(file)){
            perror("Cannot close file\n");
            exit(EXIT_FAILURE);
        }


        /* semaphore vijmaken */
        if(synchro){
            postSem(sem);
            valueSem(sem, &semVal);
            printf("PID = %d, semaphore's value = %d\n", getpid(), semVal);
        }
    }

    /* close semaphore */
    if(synchro){
        closeSem(sem);
    }

    return 0;
}
