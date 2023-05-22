/*
 * created 22/05/23 by Mateusz Glab
 * program uses queue to communication between client and server
 * first we create queue from server and then inside loop program receive
 * requests from client program, then program count math operation send answer to client
 * with open client queue, to close program type ^C
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include "queue.h"

int check = 0;
mqd_t serverMqDes, clientMqDes;
char clientName[10];

void cleanUp(void);
void signalHandler(int);


int main () {
	
	/* at the end of file */
	if (atexit (cleanUp) != 0) {
		perror("Atexit error\n");
		_exit(1);
	}

	/* in ^C case */
	if (signal (SIGINT,signalHandler) == SIG_ERR) {
		perror("Signal error\n");
		exit(EXIT_FAILURE);
	}

	/* create server queue */
	serverMqDes = createMq(MQ_NAME, O_CREAT | O_EXCL | O_RDONLY, 0666, MAX_MSG, MSG_SIZE);

	/* get attributes form queue */
	struct mq_attr attr = getAttrMq(serverMqDes);
	printf("Name: %s Des: %d Flags: %ld Maxmsg: %ld Msgsize: %ld Curmsg: %ld\n\n", MQ_NAME, serverMqDes, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	
	/* communication loop */
	while (1) {
		
		/* receive request */
		char request[MSG_SIZE];
		receiveMq (serverMqDes, request, MSG_SIZE, NULL);

		/* take values from request */
		int clientPID, num1, num2;
		char operator;
		sscanf(request, "%d%d%c%d", &clientPID, &num1, &operator, &num2);
		
		printf ("Received request form %d, request: %d %c %d\n", clientPID, num1, operator, num2);
		
		/* calculate */
		char message[MSG_SIZE];
		switch(operator){
			case '+':
				sprintf(message, "%d", num1 + num2);
				break; 
			case '-':
				sprintf (message, "%d", num1 - num2);
				break;
			case '*':
				sprintf (message, "%d", num1 * num2);
				break;
			case '/':
				sprintf (message, "%d", num1 / num2);
				break;
			default:
				sprintf (message, "Incorrect operator.");
				break;
		}

		/* open client queue */
		sprintf (request, "/%d", clientPID); 
		clientMqDes = openMq (request, O_WRONLY);
		check = 1;	//for ^C signal

		/* answer */ 
		printf ("Sending message %s to process: %d\n\n", message, clientPID);
	 	sendMq(clientMqDes, message, MSG_SIZE, 1);
		closeMq (clientMqDes);
		check = 0;	//for ^C signal
	}
	return 0;
}

void cleanUp(void) {
	closeMq(serverMqDes);
	unlinkMq(MQ_NAME);
	printf("You can close window safely\n");
}

void signalHandler(int signal) {
	printf("\nSIGINT handled\n");
	if(check) closeMq(clientMqDes);
	exit(EXIT_SUCCESS);
}
