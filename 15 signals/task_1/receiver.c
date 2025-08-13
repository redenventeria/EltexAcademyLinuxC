#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

int is_done = 0;

void handler(int signum)
{
    printf("Signal SIGUSR1 recieved\n");
    is_done = 1;
}

int main()
{
    printf("Process pid is %d\n", getpid());
    struct sigaction act = {handler, 0, 0};
    sigaction(SIGUSR1, &act, NULL);

    while(!is_done) {}
    return 0;
}