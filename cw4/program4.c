/*
 * created 18/04/23 by Mateusz Glab
 * program creates pipe and child process,
 * then the data from source file is transferred
 * using pipe to another file opened in consumer process
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SOURCE argv[1]
#define DESTINATION argv[2]

#define BUF_LEN 10  //bytes
#define DATA_FRAME_UPLOADING 9  //bytes, has to be smaller than BUF_LEN
#define DATA_FRAME_DOWNLOADING 9    //bytes, has to be smaller than BUF_LEN

int main(int argc, char *argv[]) {

    /* check input parameters */
    if (argc != 3) {
        perror("Run program correctly\n");
        printf("Type ./progName sourceFile destinationFile\n");
        exit(EXIT_FAILURE);
    }

    /* create a pipe */
    int fileDes[2];
    if (pipe(fileDes) == -1) {
        perror("Failed to create a pipe\n");
        exit(EXIT_FAILURE);
    }

    /* create child process */
    int statLock;
    switch (fork()) {
        /* fail case */
        case -1:
            perror("Fork error\n");
            exit(EXIT_FAILURE);

            /* consumer process */
        case 0:
            /* close unused pipe */
            if (close(fileDes[1]) == -1) {
                perror("Failed to close pipe\n");
                exit(EXIT_FAILURE);
            }

            /* open a destination file */
            int destinationDes;
            if ((destinationDes = open(DESTINATION, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
                perror("Failed to open destination file\n");
                exit(EXIT_FAILURE);
            }

            /* initiation of generator random numbers */
            srand(time(NULL) ^ getpid());

            /* write data from pipe to file loop */
            while (1) {

                /* sleep random time */
                sleep(rand() % 3);

                /* dynamic buffer declaration */
                char *buffer = (char *) malloc(BUF_LEN);
                if (buffer == NULL) {
                    perror("Malloc error\n");
                    exit(EXIT_FAILURE);
                }

                /* read data from pipe */
                int readen;
                if ((readen = read(fileDes[0], buffer, DATA_FRAME_DOWNLOADING)) == -1) {
                    perror("Failure to read pipe\n");
                    exit(EXIT_FAILURE);
                }

                /* break loop */
                if (readen == 0) break;

                /* write data to pipe */
                if (write(destinationDes, buffer, readen) == -1) {
                    perror("Failure to write data to destination file\n");
                    exit(EXIT_FAILURE);
                }

                /* write data to terminal */
                char *termOut = (char *) malloc(BUF_LEN + 26);
                if (termOut == NULL) {
                    perror("Malloc error\n");
                    exit(EXIT_FAILURE);
                }

                buffer[readen] = '\0';
                sprintf(termOut, "%s%s%c%c", "\x1b[35mDownloading... \x1b[0m", buffer, '\n', '\n');
                free(buffer);

                if (write(STDOUT_FILENO, termOut, BUF_LEN + 24) == -1) {
                    perror("Failure to write data to terminal\n");
                    exit(EXIT_FAILURE);
                }
                free(termOut);

            }

            /* close pipe */
            if (close(fileDes[0]) == -1) {
                perror("Failed to close pipe\n");
                exit(EXIT_FAILURE);
            }

            /* close file */
            if (close(destinationDes) == -1) {
                perror("Failed to close source file\n");
                exit(EXIT_FAILURE);
            }

            break;

            /* producer process */
        default:
            /* close unused pipe */
            if (close(fileDes[0]) == -1) {
                perror("Failed to close pipe\n");
                exit(EXIT_FAILURE);
            }

            /* open a source file */
            int sourceDes;
            if ((sourceDes = open(SOURCE, O_RDONLY, 0666)) == -1) {
                perror("Failed to open source file\n");
                exit(EXIT_FAILURE);
            }

            /* initiation of generator random numbers */
            srand(time(NULL));

            /* write data from file to pipe loop */
            while (1) {

                /* sleep random time */
                sleep(rand() % 3);

                /* dynamic buffer declaration */
                char *buffer = (char *) malloc(BUF_LEN);
                if (buffer == NULL) {
                    perror("Malloc error\n");
                    exit(EXIT_FAILURE);
                }

                /* read data from source */
                int readen;
                if ((readen = read(sourceDes, buffer, DATA_FRAME_UPLOADING)) == -1) {
                    perror("Failure to read source file\n");
                    exit(EXIT_FAILURE);
                }

                /* break loop */
                if (readen == 0) {
                    free(buffer);
                    break;
                }

                /* write data to pipe */
                if (write(fileDes[1], buffer, readen) == -1) {
                    perror("Failure to write data to pipe\n");
                    exit(EXIT_FAILURE);
                }


                /* write data to terminal */
                char *termOut = (char *) malloc(BUF_LEN + 24);
                if (termOut == NULL) {
                    perror("Malloc error\n");
                    exit(EXIT_FAILURE);
                }

                buffer[readen] = '\0';
                sprintf(termOut, "%s%s%c%c", "\x1b[34mUploading... \x1b[0m", buffer, '\n', '\n');
                free(buffer);

                if (write(STDOUT_FILENO, termOut, BUF_LEN + 24) == -1) {
                    perror("Failure to write data to terminal\n");
                    exit(EXIT_FAILURE);
                }
                free(termOut);

            }

            /* close pipe */
            if (close(fileDes[1]) == -1) {
                perror("Failed to close pipe\n");
                exit(EXIT_FAILURE);
            }

            /* close file */
            if (close(sourceDes) == -1) {
                perror("Failed to close source file\n");
                exit(EXIT_FAILURE);
            }

            /* wait for consumer process */
            if (wait(&statLock) == -1) {
                perror("Wait error\n");
                exit(EXIT_FAILURE);
            }

            break;
    }

    return 0;
}
