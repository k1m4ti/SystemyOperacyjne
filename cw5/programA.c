/*
 * Created by Mateusz Glab on 05/05/23
 * The program runs in two another processes programs which transferring data from
 * one file to another one using fifo pipe
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define FIFO_NAME fifoName
#define PRODUCER_FILE argv[1]
#define CONSUMER_FILE argv[2]
#define PRODUCER_X argv[4]
#define CONSUMER_X argv[5]

char *fifoName;
void cleanUp(void){
    unlink(fifoName);
}

int main(int argc, char *argv[]) {
    
    /* sighandler */
    if(signal(SIGINT, SIG_DFL) == SIG_ERR){
        perror("Signal error\n");
        exit(EXIT_FAILURE);
    }

    /* check number of arguments */
    if (argc != 6) {
        perror("Arguments error\n");
        exit(EXIT_FAILURE);
    }
    
    fifoName = argv[3];
    char *producer = malloc(strlen(PRODUCER_X) + 2);
    strcpy(producer, "./");
    strcat(producer, PRODUCER_X);
    char *consumer = malloc(strlen(CONSUMER_X) + 2);
    strcpy(consumer, "./");
    strcat(consumer, CONSUMER_X);

    /* create fifo file */
    if (mkfifo(FIFO_NAME, 0666) != 0) {
        perror("Can't create fifo\n");
        exit(EXIT_FAILURE);
    }

    /* atexit rejestr */
    if(atexit(cleanUp) != 0){
        perror("Atexit error\n");
        _exit(EXIT_FAILURE);
    } 
    
    int statLock = 0;

    /* first fork */
    switch (fork()) {

        /* error case */
        case -1:
            perror("Fork error\n");
            exit(EXIT_FAILURE);

            /* child process */
        case 0:

            /* second fork */
            switch (fork()) {

                /* error case */
                case -1:
                    perror("Grandson's fork error\n");
                    exit(EXIT_FAILURE);

                    /* producer process */
                case 0:
                    /* run consumer program in new process */
                    if (execlp(consumer, CONSUMER_X, FIFO_NAME, CONSUMER_FILE, NULL) != 0) {
                        perror("Execlp consumer error\n");
                        exit(EXIT_FAILURE);
                    }
                    free(consumer);
                    _exit(0);

                    /* consumer process */
                default:
                    /* run producer program in this process */
                    if (execlp(producer, PRODUCER_X, FIFO_NAME, PRODUCER_FILE, NULL) != 0) {
                        perror("Execlp producer error\n");
                        exit(EXIT_FAILURE);
                    }
                    free(producer);
                    /* wait to close child process */
                    if (wait(&statLock) == -1) {
                        perror("Wait1 error\n");
                        exit(EXIT_FAILURE);
                    }
                    _exit(0);
            }

            /* parent process */
        default:
            /* wait to close child process*/
            if (wait(&statLock) == -1) {
                perror("Wait2 error\n");
                exit(EXIT_FAILURE);
            }
            break;
    }

    return 0;
}
