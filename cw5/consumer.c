/*
* Created by Mateusz Glab on 04/05/23
* The program is transferring data from fifo pipe to consumer file
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define BUFF_LEN 10
#define DATA_FRAME 5    //has to be smaller than buffer length

#define FIFO_NAME argv[1]
#define CONSUMER_FILE argv[2]

int main(int argc, char *argv[]) {

    /* Bad input arguments in consumer program */
    if (argc != 3) {
        perror("Incorrect number of arguments in consumer program\n");
        exit(EXIT_FAILURE);
    }

    /* Open consumer file */
    int fileNum = open(CONSUMER_FILE, O_TRUNC | O_CREAT | O_WRONLY, 0666);
    if (fileNum == -1) {
        perror("Cannot open consumer file\n");
        exit(EXIT_FAILURE);
    }

    /* Open fifo pipe */
    int fifoNum = open(FIFO_NAME, O_RDONLY, 0666);
    if (fifoNum == -1) {
        perror("Cannot open fifo pipe in consumer program\n");
        exit(EXIT_FAILURE);
    }

    /* Buffer declaration */
    char buffer[BUFF_LEN];
    char termOut[BUFF_LEN + 24];

    /* Transferring loop*/
    ssize_t readData;
    while (1) {
        /* wait random time */
        sleep(rand() % 5);

        /* Read data from fifo pipe */
        if ((readData = read(fifoNum, &buffer, DATA_FRAME)) == -1) {
            perror("Failed to read fifo file in consumer program\n");
            exit(EXIT_FAILURE);
        }

        /* break loop */
        if (readData == 0) break;

        /* Write data to consumer file */
        if ((write(fileNum, &buffer, readData)) == -1) {
            perror("Cannot write data to consumer file\n");
            exit(EXIT_FAILURE);
        }

        /* Write data to terminal */
        buffer[readData] = '\0';
        sprintf(termOut, "%s%s%c", "\x1b[31mDownloading...\x1b[0m", buffer, '\n');

        if (write(STDOUT_FILENO, termOut, strlen(termOut)) == -1) {
            perror("Write to terminal is impossible, write error\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Close consumer file */
    if (close(fileNum) == -1) {
        perror("Cannot close consumer file\n");
        exit(EXIT_FAILURE);
    }

    /* Close fifo pipe */
    if (close(fifoNum) == -1) {
        perror("Cannot close fifo pipe in consumer program\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
