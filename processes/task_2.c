#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void print_info(int n)
{
    printf("Process %d   pid: %d   ppid: %d\n", n, getpid(), getppid());
}

int main()
{
    pid_t p1 = fork();
    pid_t p2 = 1;

    if(p1 != 0)
        p2 = fork();

    if(p1 == 0)
    {
        pid_t p3 = fork();
        pid_t p4 = 1;

        if(p3 != 0)
            p4 = fork();
        if(p3 == 0)
            print_info(3);
        else if(p4 == 0)
            print_info(4);
        else {
            int stat = 0;
            print_info(1);
            waitpid(p3, &stat, 0);
            waitpid(p4, &stat, 0);
        }
    }
    else if (p2 == 0) {
        pid_t p5 = fork();
        
        if(p5 == 0)
            print_info(5);
        else {
            int stat;
            print_info(2);
            waitpid(p5, &stat, 0);
        }
    }
    else {
        print_info(0);
        int stat;
        waitpid(p1, &stat, 0);
        waitpid(p2, &stat, 0);
    }

    return 0;
}