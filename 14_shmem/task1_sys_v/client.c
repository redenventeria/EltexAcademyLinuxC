#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#include"common.h"

int main()
{
    int semid = semget(SEM_KEY, 1, 0);
    if(semid == -1)
        perror("semget");
    
    int mem = shmget(SHM_KEY, 0, 0);
    if(mem == -1)
        perror("shmget");
    
    char *s = shmat(mem, NULL, 0);
    if(s == (void*)-1)
        perror("shmat");
    
    printf("Received %s\n", s);

    strcpy(s, "Hello from client!");

    struct sembuf unlock = {0, 1, 0};

    if(semop(semid, &unlock, 1) == -1)
        perror("semop");
    
    if(shmdt(s) == -1)
        perror("shmdt");

    //int memseg = shmget()

}