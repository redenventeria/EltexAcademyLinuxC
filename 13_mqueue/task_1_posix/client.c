#include<stdio.h>
#include<stdlib.h>
#include<mqueue.h>
#include<unistd.h>

int main()
{
    int mq = mq_open("/queue", O_RDWR);
    unsigned int prio = 2;

    char result[256];
    if(mq_receive(mq, result, sizeof(result), NULL) < 0)
        perror("mq_receive");
    printf("Received %s from server\n", result);

    char msg[] = "Hi!";
    if(mq_send(mq, msg, sizeof(msg), prio) < 0)
        perror("mq_send");

    mq_close(mq);
    close(mq);
    return 0;
}