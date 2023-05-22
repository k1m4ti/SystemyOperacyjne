#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


mqd_t createMq (const char *name, int oflag, mode_t mode, long mq_maxmsg, long mq_msgsize) {
	struct mq_attr attr;
	attr.mq_maxmsg = mq_maxmsg;
	attr.mq_msgsize = mq_msgsize;
	mqd_t mqDes = mq_open(name, oflag, mode, &attr);
	if (mqDes == -1) {
		perror ("Failed to create queue\n");
		exit (EXIT_FAILURE);
	}
	return mqDes;
}

mqd_t openMq (const char *name, int oflag) {
	mqd_t mqDes = mq_open(name, oflag);
	if (mqDes == -1) {
		perror ("Failed to open queue\n");
		exit (EXIT_FAILURE);
	}
	return mqDes;
}

void sendMq (mqd_t mq_des, const char *msg_ptr, size_t msg_len, unsigned int msg_prio) {
	if (mq_send(mq_des, msg_ptr, msg_len, msg_prio) == -1) {
		perror ("Failed to send message\n");
		exit (EXIT_FAILURE);
	}
}

void receiveMq (mqd_t mq_des, char *msg_ptr, size_t msg_len, unsigned int *msg_prio) {
	if (mq_receive(mq_des, msg_ptr, msg_len, msg_prio) == -1) {
		perror ("Failed to receive message\n");
		exit (EXIT_FAILURE);
	}
}

void closeMq(mqd_t mq_des) {
	if (mq_close(mq_des) == -1) {
		perror ("Failed to close queue\n");
		exit (EXIT_FAILURE);
	}
}

void unlinkMq (const char *name) {
	if (mq_unlink(name) == -1) {
		perror ("Failed to unlink queue\n");
		exit (EXIT_FAILURE);
	}
}

struct mq_attr getAttrMq(mqd_t mq_des) {
	struct mq_attr attr;
	if (mq_getattr(mq_des, &attr) == -1) {
		perror ("Failed to get attributes\n");
		exit (EXIT_FAILURE);
	}
	return attr;
}
