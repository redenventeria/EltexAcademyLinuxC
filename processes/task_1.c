#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    pid_t a = fork();
    if(a == 0)
    {
        printf("Child process    pid: %d   ppid: %d\n", getpid(), getppid());
    }
    else
    {
        printf("Parent process   pid: %d   ppid: %d\n", getpid(), getppid());
        int stat = 0;
        waitpid(a, &stat, 0);
        printf("Child process ended with code %d\n", stat);
    }
    return 0;
}