/*
 * created 05/05/23 by Mateusz Glab
 * program opens a fifio pipe and then the data
 * from source file is transferred to fifo pipe
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#define FIFO argv[1]
#define SOURCE argv[2]
#define DATA_FRAME_UPLOADING argv[3]

int main(int argc, char **argv) {

    /* check amount of parameters */
    if (argc != 4) {
        perror("Number of parameters isn't correct\n");
        printf("To run program: ./progName fifoName sourceFileName dataFrameUploading\n");
        exit(EXIT_FAILURE);
    }

    /* open source file */
    int sourceDes;
    if((sourceDes = open(SOURCE, O_RDONLY, 0666)) == -1){
        perror("Failed to open source file\n");
        exit(EXIT_FAILURE);
    }

    /* open fifo pipe */
    int fifoDes;
    if((fifoDes = open(FIFO, O_WRONLY, 0666)) == -1){
        perror("Failed to open fifo pipe\n");
        exit(EXIT_FAILURE);
    }

    /* initiation of generator random numbers */
    srand(time(NULL));

    size_t dataFrame = atoi(DATA_FRAME_UPLOADING);
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

        /* read data from source */
        ssize_t readen;
        if((readen = read(sourceDes, buffer, dataFrame)) == -1){
            perror("Failed to read data from source\n");
            exit(EXIT_FAILURE);
        }

        /* break loop */
        if(readen == 0){
            free(buffer);
            break;
        }

        /* write data to fifo */
        if(write(fifoDes, buffer, readen) == -1){
            perror("Failed to write data to fifo pipe\n");
            exit(EXIT_FAILURE);
        }

        /* write data to terminal */
        char *termOut = (char *) malloc(dataFrame + 25);
        if (termOut == NULL) {
            perror("Malloc error\n");
            exit(EXIT_FAILURE);
        }

        buffer[readen] = '\0';
        sprintf(termOut, "%s%s%c%c", "\x1b[34mUploading... \x1b[0m", buffer, '\n', '\n');
        free(buffer);

        if (write(STDOUT_FILENO, termOut, dataFrame + 24) == -1) {
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

    /* close source file */
    if(close(sourceDes) == -1){
        perror("Failed to close source file\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
