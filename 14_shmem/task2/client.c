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

#include"client_UI.h"
#include"common.h"

struct message *messages = NULL;
struct connect_msg *users = NULL;
int shm_fd;
sem_t *usr_semaphore = NULL, *msgs_semaphore = NULL;
int shm_size;
int shm_offset;

struct win *UI_msgs, *UI_users;
struct input *UI_in;

char name[16];

void init()
{
    // Getting shared memory
    shm_fd = shm_open("/users", O_RDWR, S_IWUSR | S_IRUSR);
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
    //memset(users, 0, sizeof(struct connect_msg) * MAX_USERS);

    messages = (struct message*)((char*)users + shm_offset);
    shm_offset += sizeof(struct message) * MAX_MESSAGES;

    if(messages == MAP_FAILED)
        perror("mmap messages");
    //(messages, 0, sizeof(struct message) * MAX_MESSAGES);

    if(shm_size != shm_offset) {
        printf("Shm: used memory is not equal to total memory!\n");
        exit(EXIT_FAILURE);
    }
    
    usr_semaphore = sem_open(
        "/usr_semaphore",
        0,
        S_IWUSR | S_IRUSR,
        0
    );
    if(usr_semaphore == (void*)-1)
        perror("sem_open usr_semaphore");

    msgs_semaphore = sem_open(
        "/msgs_semaphore",
        0,
        S_IWUSR | S_IRUSR,
        0
    );
    if(msgs_semaphore == (void*)-1)
        perror("sem_open msgs_semaphore");



    // Creating UI.

    UI_msgs = malloc(sizeof(struct win));
    UI_users = malloc(sizeof(struct win));
    UI_in = malloc(sizeof(struct input));

    UI_users->type = USER;
    UI_users->data = users;

    UI_msgs->type = MESSAGE;
    UI_msgs->data = messages;

    UI_init(UI_msgs, UI_users, UI_in);

    win_refresh_full(UI_msgs, UI_in);
    win_refresh_full(UI_users, UI_in);
}

void cleanup(int sig)
{
    endwin();

    printf("Cleaning up... ");

    fflush(stdout);

    //shm_unlink("/users");
    //shm_unlink("/msgs");

    //sem_unlink("/usr_semaphore");
    //sem_unlink("/msgs_semaphore");

    int i = 1;
    while(getpid() != users[i].clientid) i++;
    while(users[i].clientid != 0) {
        memcpy(&users[i], &users[i+1], sizeof(users[i + 1]));
        i++;
    }
    memcpy(&users[i], &users[i+1], sizeof(users[i + 1]));

    kill(users[0].clientid, SIGUSR1);

    munmap(users, sizeof(struct connect_msg) * MAX_USERS);
    munmap(messages, sizeof(struct message) * MAX_MESSAGES);
    users = NULL;
    messages = NULL;

    printf("Done!\n");

    exit(EXIT_SUCCESS);
}

void connect()
{
    //printf("Connecting\n");
    sem_wait(usr_semaphore);

    int i = 1;
    while(users[i].clientid != 0) i++;
    users[i].clientid = getpid();
    strcpy(users[i].name, name);
    kill(users[0].clientid, SIGUSR1);

    sem_post(usr_semaphore);
}

void update(int sig)
{
    win_refresh_full(UI_users, UI_in);
    win_refresh_full(UI_msgs, UI_in);
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Usage: client <name>\n");
        return 0;
    }
    strcpy(name, argv[1]);

    if(signal(SIGINT, cleanup) == SIG_ERR)
        perror("signal");
    if(signal(SIGUSR1, update) == SIG_ERR)
        perror("signal");
    
    init();

    connect();

    /*int a = fork();
    if(a == 0)
        connect();
    listen();

    int stat;
    while(wait(&stat) != -1) {}*/
    //printf("Server pid: %d\n", users[0].clientid);
    
    while(1) {
        char prompt[256];
        wgetstr(UI_in->w, prompt);

        if(!strcmp(prompt, "exit")) {
            kill(getpid(), SIGINT);
        }

        int i = 1;
        while(messages[i].name[0] != 0) i++;
        strcpy(messages[i].text, prompt);
        strcpy(messages[i].name, name);
        werase(UI_in->w);
        wmove(UI_in->w, 0, 0);
        wrefresh(UI_in->w);
        kill(users[0].clientid, SIGUSR1);
    }

    cleanup(0);
    return 0;
}