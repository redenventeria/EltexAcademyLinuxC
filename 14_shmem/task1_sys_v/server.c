#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<string.h>

#include"common.h"

int main()
{
    int memseg = shmget(SHM_KEY, 256, IPC_CREAT | S_IWUSR | S_IRUSR);
    if(memseg == -1)
        perror("shmget");

    /*int mem = shmget(memseg, 256, 0);
    if(mem == -1)
        perror("shmget");*/

    char *a = shmat(memseg, NULL, 0);
    if((void*)a == (void*)-1)
        perror("shmat");
    
    strcpy(a, "Hello from server!");

    int sem = semget(SEM_KEY, 1,  S_IWUSR | S_IRUSR | IPC_CREAT);
    if(sem == -1)
        perror("semget");

    struct sembuf wait = {0, -1, 0};
    
    
    if(semop(sem, &wait, 1) == -1)
        perror("semop");
    
    printf("Received %s\n", a);
    
    if(shmdt(a) == -1)
        perror("shmdt");
    
    if(shmctl(memseg, IPC_RMID, NULL))
        perror("scmctl");
    if(semctl(sem, 0, IPC_RMID) == -1)
        perror("semctl");

    return 0;
}