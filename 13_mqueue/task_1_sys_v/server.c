#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/fcntl.h>
#include<string.h>

#include "common.h"

int main()
{
    int queue = msgget(1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(queue == -1)
        perror("msgget");
    
    struct msgbuf msg;
    msg.mtype = 2;
    strncpy(msg.buf, "Hello!", sizeof(msg.buf));

    int d = msgsnd(queue, &msg, sizeof(msg.buf), 0);
    if(d == -1)
        perror("msgsnd");
    
    struct msgbuf result;
    int a = msgrcv(queue, &result, sizeof(result.buf), 1, 0);
    if(a == -1)
        perror("msgrcv");
    printf("Received %s from client\n", result.buf);

    msgctl(queue, IPC_RMID, NULL);
    

    return 0;
}