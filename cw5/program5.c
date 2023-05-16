/*
 * created 05/05/23 by Mateusz Glab
 * program creates fifio pipe and child process,
 * then the data from source file is transferred
 * using pipe to another file opened in consumer process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PRODUCER_PATH argv[1]
#define CONSUMER_PATH argv[2]
#define PRODUCER argv[3]
#define CONSUMER argv[4]
#define FIFO argv[5]
#define SOURCE argv[6]
#define DESTINATION argv[7]
#define DATA_FRAME_UPLOADING argv[8]
#define DATA_FRAME_DOWNLOADING argv[9]


char *fifoName;

void cleanUp(void);

void signalHandler(int);

int main(int argc, char **argv) {

    /* check amount of parameters */
    if (argc != 10) {
        perror("Number of parameters isn't correct\n");
        printf("To run program: ./progName ./prodName ./consName prodName consName"
               "fifoName sourceFileName consumerFileName\n");
        exit(EXIT_FAILURE);
    }

    /* fifo name */
    fifoName = FIFO;

    /* to end of program */
    if (atexit(cleanUp) != 0) {
        perror("Atexit error\n");
        _exit(1);
    }

    /* in ^C case */
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("Signal error\n");
        _exit(1);
    }

    /* create fifo */
    if (mkfifo(fifoName, 0666) == -1) {
        perror("Cannot create fifo\n");
        exit(EXIT_FAILURE);
    }

    int statLock;

    for (int i = 0; i < 2; i++) {
        switch (fork()) {
            case -1:
                perror("Fork error\n");
                exit(EXIT_FAILURE);

            case 0:
                /* open producer program */
                if (i == 0)
                    if (execlp(PRODUCER_PATH, PRODUCER, FIFO, SOURCE, DATA_FRAME_UPLOADING, NULL) == -1) {
                        perror("Can't run producer program\n");
                        exit(EXIT_FAILURE);
                    }

                /* open consumer program */
                if (i == 1)
                    if (execlp(CONSUMER_PATH, CONSUMER, FIFO, DESTINATION, DATA_FRAME_DOWNLOADING, NULL) == -1) {
                        perror("Can't run consumer program\n");
                        exit(EXIT_FAILURE);
                    }
                break;

            default:
                break;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (wait(&statLock) == -1) {
            perror("Fork error\n");
            exit(EXIT_FAILURE);
        }
    }


    return 0;
}


void cleanUp(void) {
    printf("Unlinking fifo\n");
    if (unlink(fifoName) == -1) {
        perror("Failed to unlink fifo\n");
        _exit(1);
    }
}

void signalHandler(int signal) {
    printf("SIGINT handled, unlinking fifo\n");
    if (unlink(fifoName) == -1) {
        perror("Failed to unlink fifo\n");
        _exit(1);
    }
}
