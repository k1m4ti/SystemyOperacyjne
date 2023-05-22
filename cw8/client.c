/*
 * created 22/05/23 by Mateusz Glab
 * program uses queue to communication between client and server
 * first we open queue to server and then inside loop program send
 * requests to server, to close program type ^D or ^C
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include "queue.h"

int check = -1;
mqd_t serverMqDes, clientMqDes;
char clientName[10];

void cleanUp(void);

void signalHandler(int); 

int main () {

	/* at the end of program */		
	if (atexit (cleanUp) != 0) {
		perror("atexit error");
		_exit(1);
	}
	
	/* in ^c case */
	if (signal (SIGINT,signalHandler) == SIG_ERR) {
		perror("Błąd signal");
		exit(EXIT_FAILURE);
	}

	/* wait for server */
	sleep(1);

	/* open server queue */
	serverMqDes = openMq (MQ_NAME, O_WRONLY);
	
	/* get attributes form queue */
	struct mq_attr attr = getAttrMq (serverMqDes);
	printf("Name: %s Des: %d Flags: %ld Maxmsg: %ld Msgsize: %ld Curmsg: %ld\n\n", MQ_NAME, serverMqDes, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);		
	
	/* create queue name */
	pid_t pID = getpid();
	sprintf (clientName, "/%d", pID);
	
	printf("You can only use [+, -, *, /]\n");

	/* comunication loop */
	while (1) {
	
		printf ("\nType operation:\n");
		char message[MSG_SIZE - 10];
		if (scanf ("%s", message) == EOF) {
			printf ("End of file\n");
			exit(EXIT_SUCCESS);
		}

		/* add PID to request */
		char request[MSG_SIZE];
		sprintf(request, "%d %s", pID, message);
		
		/* send request to server */
		sendMq(serverMqDes, request, MSG_SIZE, 0);
		printf ("\nSent request: %s\n", request);
		
		/* create client queue */
		clientMqDes = createMq (clientName, O_CREAT | O_EXCL | O_RDONLY, 0666, MAX_MSG, MSG_SIZE);
		
		/* receive answer from server */
		receiveMq(clientMqDes, request, MSG_SIZE, NULL);
		printf ("Answer from server: %s\n\n", request);

		check = 0;	//for ^C signal
		/* close client queue */
		closeMq (clientMqDes);
		check = 1; //for ^C signal
		/* unlink client queue */
		unlinkMq(clientName);
		check = -1; //for ^C signal
	}
	return 0;
}

void cleanUp(void) {
	closeMq (serverMqDes);
	printf("You can close window safely\n");
}

void signalHandler(int signal) {
	printf ("\nSIGINT handled\n");
	if(check == 0){
		closeMq(clientMqDes);
		unlinkMq(clientName);
	}

	if(check == 1) unlinkMq(clientName);
	
	exit(EXIT_SUCCESS);
}
