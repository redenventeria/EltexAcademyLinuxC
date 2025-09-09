#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/fcntl.h>
#include<string.h>

#include "common.h"

int main()
{
    int queue = msgget(1, S_IRUSR | S_IWUSR);
    if(queue == -1)
        perror("msgget");

    struct msgbuf result;
    memset(&result, 0, sizeof(result));
    int a = msgrcv(queue, &result, sizeof(result.buf), 2, 0);
    if(a == -1) 
        perror("msgrcv");
    
    struct msgbuf msg;
    msg.mtype = 1;
    strncpy(msg.buf, "Hi!", sizeof(msg.buf));

    if(msgsnd(queue, &msg, sizeof(msg.buf), 0) == -1)
        perror("msgsnd");
    
    printf("Received %s from server\n", result.buf);

    return 0;
}