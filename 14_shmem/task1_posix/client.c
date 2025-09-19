#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_NAME "/posix_shm_example"
#define SEM_SERVER_NAME "/posix_sem_server"
#define SEM_CLIENT_NAME "/posix_sem_client"
#define SHM_SIZE 1024

int main() {
    int shm_fd;
    char *ptr;
    sem_t *sem_server, *sem_client;

    shm_fd = shm_open(SHM_NAME, O_RDWR, S_IWUSR | S_IRUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_server = sem_open(SEM_SERVER_NAME, 0);
    sem_client = sem_open(SEM_CLIENT_NAME, 0);
    if (sem_server == SEM_FAILED || sem_client == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    sem_wait(sem_server);
    
    printf("Received %s\n", ptr);
    
    strcpy(ptr, "Hello!");
    
    sem_post(sem_client);
    
    sem_close(sem_server);
    sem_close(sem_client);
    
    munmap(ptr, SHM_SIZE);
    
    close(shm_fd);
    
    return 0;
}