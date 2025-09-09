#include<stdio.h>
#include<stdlib.h>
#include<mqueue.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 10;

    mqd_t mq = mq_open(
        "/queue", 
        O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR,
        &attr
    );
    if(mq == -1)
        perror("mq_open");

    unsigned int prio = 1;
    char msg[] = "Hello!";
    if(mq_send(mq, msg, sizeof(msg), prio) == -1)
        perror("mq_send");
    
    sleep(5);
    
    char result[256];
    int a = mq_receive(mq, result, sizeof(result), NULL);
    printf("%d\n", a);
    if(a != 0)
        perror("mq_receive");
    printf("Received %s from client\n", result);

    if(mq_unlink("/queue") == -1)
        perror("mq_unlink");
    return 0;
}