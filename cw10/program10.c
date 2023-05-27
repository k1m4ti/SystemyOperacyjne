/*
    created by Mateusz Glab 27/05/23
    The program implements the bakery algorithm to synchronize threads
    in a critical section, ensuring exclusive access to the num variable.
    Threads assign themselves bakery numbers and then perform operations 
    on num within the critical section, maintaining order based on the assigned numbers.
*/

#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "thread.h"

int numThreads, numSections, num = 0;

volatile int *choosing, *number;

pthread_t *threads;

int max_number(void);
void lock(int tIDs);
void unlock(int tIDs);
void *thread_function(void *arg);


int main(int argc, char **argv)
{
    /* check amount of parameters */
    if (argc != 3) {
        perror("Incorrect number of parameters\n");
        printf("To run program correctly type: ./ProgName NumberOfThreads NumberOfSections\n");
        exit(EXIT_FAILURE);
    }

    /* convert stirng to int */
    numThreads = atoi(argv[1]);
    numSections = atoi(argv[2]);
    
    /* allocate memory for choosing tab */
    choosing = (volatile int *) malloc(sizeof(int) * numThreads);
    if(choosing == NULL){
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    /* allocate memory for number tab */
    number = (volatile int *) malloc(sizeof(volatile int) * numThreads);
    if(number == NULL){
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    /* initialize arrays to zero values */
    for (int i = 0; i < numThreads; i++)
    {
        choosing[i] = 0;
        number[i] = 0;
    }

    /* allocate memory for threads tab */
    threads = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);
    if(threads == NULL){
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate memory for IDs tab */
    int *tIDs = (int *) malloc(sizeof(int) * numThreads); 
    if(tIDs == NULL){
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    /* create threads */
    for (int i = 0; i < numThreads; i++)
    {
        tIDs[i] = i;
        threads[i] = createPth(thread_function, (void *)&tIDs[i]);
    }

    /* wait for end of threads */
    for (int i = 0; i < numThreads; i++)
        joinPth(threads[i], NULL);


    /* free allocated memory */
    free((void *) threads);
    free((void *) tIDs);
    free((void *) choosing);
    free((void *) number);

    /* change position of cursor */ 
    hoverOver(0, 4 + 2 * numThreads);

    /* print a result */
    if (num == numThreads * numSections)
        printf("Synchronization was succesfull\n");
    else
        printf("Synchronization wasn't succesfull\n");    
    
    return 0;
}

int max_number(void)
{
    int max = number[0];
    int i;
    for (i = 1; i < numThreads; i++)
    {
        if (number[i] > max)
        {
            max = number[i];
        }
    }
    return max;
}

void lock(int tIDs)
{
    choosing[tIDs] = 1;
    number[tIDs] = 1 + max_number();
    choosing[tIDs] = 0;

    int i;
    for (i = 0; i < numThreads; i++)
    {
        if (i != tIDs)
        {
            while (choosing[i])
            {
                /*  wait for choice */
            }

            while (number[i] != 0 && (number[i] < number[tIDs] || (number[i] == number[tIDs] && i < tIDs)))
            {
                /* Wait for another thread  */
            }
        }
    }
}

void unlock(int tIDs)
{
    number[tIDs] = 0;
}

void *thread_function(void *arg)
{
    /* casting types */
    int tIDs = *(int *)arg;

    /* print thread nr and ID */
    printf("Thread nr: %d ID: %p\n", tIDs, (void *) threads[tIDs]);

    /* sleep the random amount of time */
    srand(time(NULL));
    sleep(rand() % 3);

    /* sections loop */
    for(int i = 0, count; i < numSections; i++){

        /* change position of cursor */
        hoverOver(0, 4 + numThreads + tIDs);

        /* sleep the random amount of time */
        sleep(rand() % 3);
        
        /* critical section */
        lock(tIDs);

        /* change position of cursor */
        hoverOver(25, 4 + numThreads + tIDs);

        /* print thread  values in crtitical section */
        printf("\033[35mThread nr: %d, CriticalSession: %d, Num: %d \033[0m\n", tIDs, i, num);

        /* assign the value of num to count */
        count = num;

        /* increment the value of count */
        count ++;

        /* sleep the random amount of time */
        sleep(rand() % 3);

        /* assign the updated value of count back to num */
        num = count;

        /* end of critical section */
        unlock(tIDs);

        /* change position of cursor */
        hoverOver(0, 4 + numThreads + tIDs);

        /* print thread  values in private section */
        printf("\033[32mThread nr: %d, PrivateSession: %d, Num: %d \033[0m\n", tIDs, i, num);
        
        /* sleep the random amount of time */
        sleep(rand() % 3);
    }

    /* exit thread */
    pthread_exit(NULL);
}
