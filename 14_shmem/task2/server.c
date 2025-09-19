#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include<fcntl.h>

#include"list.h"
#include"common.h"

struct message *messages = NULL;
struct connect_msg *users = NULL;
int shm_fd;
sem_t *usr_semaphore = NULL, *msgs_semaphore = NULL;
int shm_size;
int shm_offset = 0;

void init()
{
    // Getting shared memory
    shm_fd = shm_open("/users", O_CREAT | O_EXCL | O_RDWR, S_IWUSR | S_IRUSR);
    if(shm_fd == -1)
        perror("shm_open usr_memory");
    
    shm_size = sizeof(struct connect_msg) * MAX_USERS + 
               sizeof(struct message)     * MAX_MESSAGES;

    if(ftruncate(shm_fd, shm_size) == -1)
        perror("ftruncate");



    // Distributing the memory.
    users = mmap(
        NULL,
        shm_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        shm_offset
    );
    shm_offset += sizeof(struct connect_msg) * MAX_USERS;

    if(users == MAP_FAILED)
        perror("mmap users");
    memset(users, 0, sizeof(struct connect_msg) * MAX_USERS);

    messages = (struct message*)((char*)users + shm_offset);
    shm_offset += sizeof(struct message) * MAX_MESSAGES;

    memset(messages, 0, sizeof(struct message) * MAX_MESSAGES);

    if(shm_size != shm_offset) {
        printf("Shm: used memory is not equal to total memory!\n");
        exit(EXIT_FAILURE);
    }



    // Inserting server info.
    users[0].clientid = getpid();
    strcpy(users[0].name, "server");



    // Creating semaphores.
    usr_semaphore = sem_open(
        "/usr_semaphore",
        O_CREAT | O_EXCL,
        S_IWUSR | S_IRUSR,
        1
    );
    if(usr_semaphore == (void*)-1)
        perror("sem_open usr_semaphore");

    msgs_semaphore = sem_open(
        "/msgs_semaphore",
        O_CREAT | O_EXCL,
        S_IWUSR | S_IRUSR,
        1
    );
    if(msgs_semaphore == (void*)-1)
        perror("sem_open msgs_semaphore");

}

void notify(int sig) {

    printf("Notifying clients...\n");

    if(sem_wait(usr_semaphore) == -1)
        perror("sem_wait");
    if(sem_wait(msgs_semaphore) == -1)
        perror("sem_wait");
    
    int i = 1; 
    while(users[i].clientid != 0) {
        kill(users[i].clientid, SIGUSR1);
        i++;
    }

    if(sem_post(usr_semaphore) == -1)
        perror("sem_wait");
    if(sem_post(msgs_semaphore) == -1)
        perror("sem_wait");
    
}

void cleanup(int sig)
{
    printf("Cleaning up... ");
    fflush(stdout);
    munmap(users, sizeof(struct connect_msg) * MAX_USERS);
    munmap(messages, sizeof(struct message) * MAX_MESSAGES);
    users = NULL;
    messages = NULL;

    shm_unlink("/users");
    shm_unlink("/msgs");

    sem_unlink("/usr_semaphore");
    sem_unlink("/msgs_semaphore");

    printf("Done!\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    if(signal(SIGINT, cleanup) == SIG_ERR)
        perror("signal");
    if(signal(SIGUSR1, notify) == SIG_ERR)
        perror("signal");
    
    init();

    /*int a = fork();
    if(a == 0)
        connect();
    listen();

    int stat;
    while(wait(&stat) != -1) {}*/
    
    printf("Server pid: %d\n", users[0].clientid);
    while(1) sleep(100);

    cleanup(0);
    return 0;
}