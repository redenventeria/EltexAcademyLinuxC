#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

int is_done = 0;

void handler(int signum)
{
    printf("Signal SIGINT recieved, its number is %d\n", signum);
    is_done = 1;
}

int main()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL);
    printf("Blocked SIGINT\n");

    printf("Process pid is %d\n", getpid());

    struct sigaction act = {handler, 0, 0};
    sigaction(SIGINT, &act, NULL);
    printf("Registered handler for SIGINT\n");

    while(!is_done) {}

    return 0;
}