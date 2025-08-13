#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: ./sender <pid>\n");
        return EXIT_FAILURE;
    }

    pid_t proc = atoi(argv[1]);
    kill(proc, SIGUSR1);
    printf("Sent SIGUSR1 to process %d\n", proc);
    return 0;
}