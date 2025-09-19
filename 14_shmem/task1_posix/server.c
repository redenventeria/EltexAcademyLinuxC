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

    sem_unlink(SEM_SERVER_NAME);
    sem_unlink(SEM_CLIENT_NAME);
    shm_unlink(SHM_NAME);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_server = sem_open(SEM_SERVER_NAME, O_CREAT, S_IWUSR | S_IRUSR, 0);
    sem_client = sem_open(SEM_CLIENT_NAME, O_CREAT, S_IWUSR | S_IRUSR, 0);
    if (sem_server == SEM_FAILED || sem_client == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    strcpy(ptr, "Hi!");
    
    sem_post(sem_server);
    
    sem_wait(sem_client);
    
    printf("Received %s\n", ptr);
    
    sem_close(sem_server);
    sem_close(sem_client);
    sem_unlink(SEM_SERVER_NAME);
    sem_unlink(SEM_CLIENT_NAME);
    
    munmap(ptr, SHM_SIZE);
    
    close(shm_fd);
    
    shm_unlink(SHM_NAME);
    
    return 0;
}