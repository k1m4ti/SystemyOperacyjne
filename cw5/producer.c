/*
*  Created by Mateusz Glab on 03/05/23
*  The program is transferring data form producer file to fifo pipe
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define BUFF_LEN 10
#define DATA_FRAME 5 //has to be smaller than BUFF_LEN

#define FIFO_NAME argv[1]
#define PRODUCER_FILE argv[2]

int main(int argc, char *argv[]) {

    /* Bad input arguments in producer program */
    if (argc != 3) {
        perror("Incorrect number of arguments in producer program\n");
        exit(EXIT_FAILURE);
    }

    /* Open producer file*/
    int fileNum = open(PRODUCER_FILE, O_RDONLY, 0666);
    if (fileNum == -1) {
        perror("Cannot open producer file\n");
        exit(EXIT_FAILURE);
    }

    /* Open fifo pipe */
    int fifoNum = open(FIFO_NAME, O_WRONLY, 0666);
    if (fifoNum == -1) {
        perror("Cannot open fifo pipe in producer program\n");
        exit(EXIT_FAILURE);
    }

    /* Buffer declaration */
    char buffer[BUFF_LEN];
    char termOut[BUFF_LEN + 24];

    /* transferring loop */
    ssize_t readData;
    while (1) {
        /* wait random time */
        sleep(rand() % 5);

        /* read data from file*/
        if ((readData = read(fileNum, &buffer, DATA_FRAME)) == -1) {
            perror("Failed to read producer file\n");
            exit(EXIT_FAILURE);
        }

        /* break loop */
        if (readData == 0) break;

        /* write to fifo pipe */
        if (write(fifoNum, &buffer, readData) == -1) {
            perror("Write to fifo pipe is impossible, write error\n");
            exit(EXIT_FAILURE);
        }

        /* write to terminal */
        buffer[readData] = '\0';
        sprintf(termOut, "%s%s%c", "\x1b[32mUploading...\x1b[0m", buffer, '\n');

        if (write(STDOUT_FILENO, termOut, strlen(termOut)) == -1) {
            perror("Write to terminal is impossible, write error\n");
            exit(EXIT_FAILURE);
        }

    }

    /* close producer file */
    if (close(fileNum) == -1) {
        perror("Cannot close producer file\n");
        exit(EXIT_FAILURE);
    }

    /* close fifo pipe */
    if (close(fifoNum) == -1) {
        perror("Cannot close fifo pipe in producer program\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
