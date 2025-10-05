#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<malloc.h>
#include<errno.h>
#include<time.h>

#define NEW_TASK 1
#define GET_STATE 2
#define AVALIABLE 3
#define BUSY 4

#define MAX_DRIVERS 100

typedef struct {
    long mtype;
    char buf[64];
} msgbuf;

int main_mq;

pid_t drivers[MAX_DRIVERS] = {0};
int drivers_mq[MAX_DRIVERS] = {0};
size_t drivers_count = 0;

int get_mq_by_pid(pid_t pid) {

    size_t i = 0;
    while(i < drivers_count && drivers[i] != pid)
        i++;
    if (i == drivers_count) {
        return -1;
    }

    return drivers_mq[i];

}

void driver(int mq) {

    time_t task_start = 0, time_left = 0;
    int task_len = 0;
    msgbuf msg;

    while(1) {

        if(msgrcv(mq, &msg, sizeof(msg.buf), 0, 0) == -1) {
            if(errno == EIDRM) 
                exit(EXIT_SUCCESS);
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        time_t cur_time = time(NULL);
        msgbuf response;
        memset(&response, 0, sizeof(response));

        switch(msg.mtype) {

            case NEW_TASK:

                
                time_left = task_len - (cur_time - task_start);
                if(time_left <= 0) {

                    memcpy(&task_len, msg.buf, sizeof(task_len));
                    task_start = time(NULL);

                    response.mtype = AVALIABLE;
                    msgsnd(main_mq, &response, sizeof(response.buf), 0);
                }
                else {
                    response.mtype = BUSY;
                    memcpy(response.buf, &time_left, sizeof(time_left));
                    msgsnd(main_mq, &response, sizeof(response.buf), 0);
                }

                break;

            case GET_STATE:
                time_left = task_len - (cur_time - task_start);

                if(time_left <= 0) {
                    response.mtype = AVALIABLE;
                    msgsnd(main_mq, &response, sizeof(response.buf), 0);
                }
                else {
                    response.mtype = BUSY;
                    memcpy(response.buf, &time_left, sizeof(time_left));
                    msgsnd(main_mq, &response, sizeof(response.buf), 0);
                }

                break;
        }

    }
}

void create_driver() {

    if(drivers_count >= 100) {
            printf("Too many drivers!\n");
            return;
    }

    int driver_mq = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);

    printf("Creating driver...\n");
    pid_t driver_pid = fork();
    if (driver_pid == 0) {
        driver(driver_mq);
        exit(EXIT_SUCCESS);
    }
    else {
        drivers[drivers_count] = driver_pid;
        drivers_mq[drivers_count] = driver_mq;
        drivers_count++;

        printf("New driver pid is %d\n", driver_pid);
    }
}

void send_task(pid_t pid, int length) {

    int mq = get_mq_by_pid(pid);
    if(mq == -1) {
        printf("Error: no driver with such pid!\n");
        return;
    }

    msgbuf msg = {NEW_TASK, {0}};
    memcpy(msg.buf, &length, sizeof(length));
    if(msgsnd(mq, &msg, sizeof(msg.buf), 0) == -1)
        perror("msgsnd");

    msgbuf response;
    if(msgrcv(main_mq, &response, sizeof(response.buf), 0, 0) == -1)
        perror("msgrcv");

    if(response.mtype == BUSY) {
        int time_left;
        memcpy(&time_left, response.buf, sizeof(time_left));
        printf("Driver busy! %d seconds left.\n", time_left);
    }
}

void status_driver(pid_t pid) {
    int mq = get_mq_by_pid(pid);

    if(mq == -1) {
        printf("Error: no driver with such pid!\n");
        return;
    }

    msgbuf msg = {GET_STATE, {0}};
    if(msgsnd(mq, &msg, sizeof(msg.buf), 0) == -1) {
        perror("msgsnd");
    }

    msgrcv(main_mq, &msg, sizeof(msg.buf),  0, 0);

    if(msg.mtype == AVALIABLE)
        printf("Driver with pid %d is avaliable\n", pid);
    if(msg.mtype == BUSY) {
        int time_left;
        memcpy(&time_left, msg.buf, sizeof(time_left));
        printf("Driver with pid %d is busy, %d seconds left\n", pid, time_left);
    }

    return;

}

void get_drivers() {

    for(size_t i = 0; i < drivers_count; i++) {
        status_driver(drivers[i]);
    }
}

int main()
{
    // Создать структуры для хранения информации о драйверах.
    // Для каждого драйвера достаточно знать его pid, по нему обращаться к нему.

    main_mq = msgget(IPC_PRIVATE, S_IWUSR | S_IRUSR);

    printf("Usage:\ncreate_driver\nsend_task <pid>\nstatus_driver <pid>\nget_drivers\nexit\n");

    char *input = NULL;
    size_t input_size = 0;

    while(1) {

        printf(">");
        fflush(stdout);

        char command[50];
        int arg_count;
        int arg1, arg2;

        getline(&input, &input_size, stdin);
        arg_count = sscanf(input, "%49s %d %d", command, &arg1, &arg2);
        if(arg_count < 1) {
            printf("command is not read!\n");
            continue;
        }

        if(!strcmp(command, "create_driver")) {
            if(arg_count != 1)
                printf("create_driver does not require additional arguments!\n");
            else
                create_driver();
        }
        else if(!strcmp(command, "send_task")) {
            if(arg_count != 3)
                printf("send_task requires two additional arguments!\n");
            else
                send_task(arg1, arg2);
        }
        else if(!strcmp(command, "status_driver")) {
            if(arg_count != 2)
                printf("status_driver requires one additional argument!\n");
            status_driver(arg1);
        }
        else if(!strcmp(command, "get_drivers")) {
            if(arg_count != 1)
                printf("get_drivers does not require additional arguments!\n");
            get_drivers();
        }
        else if(!strcmp(command, "exit")) {
            break;
        }
        else
            printf("No such command!\n");
    }

    // Освободить память, удалить все очереди сообщений, завершить процессы:
    for(size_t i = 0; i < drivers_count; i++) {
        msgctl(drivers_mq[i], IPC_RMID, NULL);
        int stat = 0;
        waitpid(drivers[i], &stat, 0);
    }

    msgctl(main_mq, IPC_RMID, NULL);
    free(input);

    return 0;
}
