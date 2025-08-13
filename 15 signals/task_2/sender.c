#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
    pid_t proc = atoi(argv[1]);
    kill(proc, SIGINT);
    printf("Sent SIGINT to process %d\n", proc);
    return 0;
}