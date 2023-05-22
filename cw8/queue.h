#ifndef QUEUE_H
#define QUEUE_H

#include <mqueue.h>
#include <stddef.h>

#define MQ_NAME "/queue"
#define MAX_MSG 10
#define MSG_SIZE 30

/* function to create queue with error handling, return queue's descriptor */
mqd_t createMq (const char *name, int oflag, mode_t mode, long mq_maxmsg, long mq_msgsize);

/* function to open exising queue, return queue's descriptor */
mqd_t openMq (const char *name, int oflag);

/* function to close queue */
void closeMq (mqd_t mq_des);

/* function to unlink queue */
void unlinkMq(const char *name);

/* function to get attributes, return struct */
struct mq_attr getAttrMq(mqd_t mq_des);

/* function to send messages */
void sendMq (mqd_t mq_des, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

/* function to receive messages */
void receiveMq (mqd_t mq_des, char *msg_ptr, size_t msg_len ,unsigned int *msg_prio);

#endif
