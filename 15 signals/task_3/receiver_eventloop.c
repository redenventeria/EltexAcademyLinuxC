#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    printf("Receiver pid is %d\n", getpid());

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    printf("Blocked USRSIG1\n");

    while(1) {
        printf("Waiting for SIGUSR1...\n");
        int sig;
        sigwait(&set, &sig);
        printf("Signal SIGINT recieved\n");
    }

    return 0;
}