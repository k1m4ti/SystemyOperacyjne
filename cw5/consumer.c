/*
 * created 05/05/23 by Mateusz Glab
 * program opens a fifio pipe and then the data
 * from pipe is transferred to file
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#define FIFO argv[1]
#define DESTINATION argv[2]
#define DATA_FRAME_DOWNLOADING argv[3]

int main(int argc, char **argv) {

    /* check amount of parameters */
    if (argc != 4) {
        perror("Number of parameters isn't correct\n");
        printf("To run program: ./progName fifoName destinationFileName dataFrameDownloading\n");
        exit(EXIT_FAILURE);
    }

    /* open source file */
    int destinationDes;
    if((destinationDes = open(DESTINATION, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1){
        perror("Failed to open/create destination file\n");
        exit(EXIT_FAILURE);
    }

    /* open fifo pipe */
    int fifoDes;
    if((fifoDes = open(FIFO, O_RDONLY, 0666)) == -1){
        perror("Failed to open fifo pipe\n");
        exit(EXIT_FAILURE);
    }

    /* initiation of generator random numbers */
    srand(time(NULL) ^ getpid());

    size_t dataFrame = atoi(DATA_FRAME_DOWNLOADING);
    /* write data to fifo loop */
    while(1){

        /* wait random time */
        sleep(rand() % 3);

        /* dynamic buffer declaration */
        char *buffer = (char *) malloc(dataFrame);
        if(buffer == NULL){
            perror("Malloc error\n");
            exit(EXIT_FAILURE);
        }

        /* read data from fifo pipe */
        ssize_t readen;
        if((readen = read(fifoDes, buffer,dataFrame)) == -1){
            perror("Failed to read data from source\n");
            exit(EXIT_FAILURE);
        }

        /* break loop */
        if(readen == 0){
            free(buffer);
            break;
        }

        /* write data to file */
        if(write(destinationDes, buffer, readen) == -1){
            perror("Failed to write data to destination file\n");
            exit(EXIT_FAILURE);
        }

        /* write data to terminal */
        char *termOut = (char *) malloc(dataFrame + 27);
        if (termOut == NULL) {
            perror("Malloc error\n");
            exit(EXIT_FAILURE);
        }

        buffer[readen] = '\0';
        sprintf(termOut, "%s%s%c%c", "\x1b[35mDownloading... \x1b[0m", buffer, '\n', '\n');
        free(buffer);

        if (write(STDOUT_FILENO, termOut, dataFrame + 26) == -1) {
            perror("Failure to write data to terminal\n");
            exit(EXIT_FAILURE);
        }
        free(termOut);
    }

    /* close fifo pipe */
    if(close(fifoDes) == -1){
        perror("Failed to close fifo pipe\n");
        exit(EXIT_FAILURE);
    }

    /* close destination file */
    if(close(destinationDes) == -1){
        perror("Failed to close source file\n");
        exit(EXIT_FAILURE);
    }

    return 1;
}
