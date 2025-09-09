#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include<sys/msg.h>
#include<sys/stat.h>

#include"server.h"
#include"common.h"
#include"list.h"

struct list msgs;
struct list users;
int id;

int init_msgqueue() {
    int id = msgget(SERVER_CONNECT_KEY, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    list_init(&msgs, sizeof(struct message));
    list_init(&users, sizeof(struct connect_msg));

    return id;
}

void server_listener(int id) {
    while(1) {

        struct msgbuf a;
        printf("Ready to receive next request\n");
        memset(&a, 0, sizeof(a));
        if(msgrcv(id, &a, sizeof(a.data), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        else
            printf("Next request received\n");

        switch (a.mtype) { 

            case CLIENT_INIT:
                printf("Accepting connection from %s\n", ((struct connect_msg*)a.data)->name);
                accept_con((struct connect_msg*)a.data);
                break;
            case CLIENT_MSG:
                printf("Received message from %s\n", ((struct message*)a.data)->name);
                receive((struct message*)a.data);
                break;
            case CLIENT_CLOSE:
                printf("Closing connection\n");
                close_con((struct connect_msg*)a.data);
                break;
            default:
                printf("Unreachable\n");
        }
    }
}

void accept_con(struct connect_msg *a) {

    printf("accepted %s\n", a->name);
    send_chat_info(a);
    list_add(&users, a);
    broadcast_new_user(a, SERVER_NEWUSR); 
}

void receive(struct message *a) {
    
    list_add(&msgs, a);
    broadcast_new_msg(a);

}

void close_con(struct connect_msg *a) {
    list_del(&users, a, comp_con);
    printf("Size of users is now %lu\n", users.n);
    broadcast_new_user(a, SERVER_DISCONNECT_USR);
}

int broadcast_new_msg(struct message *a) {

    for(size_t i = 0; i < users.n; i++) {

        int clientid = ((struct connect_msg*)users.data[i])->clientid;
        inform_msg(clientid, a);
    }

    return 0;
}

int broadcast_new_user(struct connect_msg *a, int type) {

    printf("broadcast_new_usr:\n");
    for(size_t i = 0; i < users.n; i++) {

        int clientid = ((struct connect_msg*)users.data[i])->clientid;
        printf("clientid:%d\n", clientid);
        inform_usr(clientid, a, type);
    }

    return 0;
}



int inform_msg(int clientid, struct message *a) {

    struct msgbuf b;
    b.mtype = SERVER_NEWMSG;
    memcpy(b.data, a, sizeof(*a));

    if(msgsnd(clientid, &b, msgs.elem_size, 0) == -1)
        perror("msgsnd");
    
    printf("Sent new message to client with id %d\n", clientid);
    return 0;
}


int inform_usr(int clientid, struct connect_msg *a, int type) {
    struct msgbuf b;
    b.mtype = type;
    memcpy(b.data, a, sizeof(*a));
    ((struct connect_msg*)&b.data)->clientid = 0;

    if(msgsnd(clientid, &b, users.elem_size, 0) == -1)
        perror("msgsnd");
    else
        printf("Sent info about user %s\n", a->name);

    return 0;
}


int send_chat_info(struct connect_msg *a) {

    printf("send_chat_info:\n");
    for(size_t i = 0; i < msgs.n; i++) {
        inform_msg(a->clientid, (struct message*)msgs.data[i]);
    }
    
    for(size_t i = 0; i < users.n; i++) {
        inform_usr(a->clientid, (struct connect_msg*)users.data[i], SERVER_NEWUSR);
    }
    
    return 0;

}

int main()
{
    int id = init_msgqueue();
    server_listener(id);

    msgctl(id, IPC_RMID, NULL);

    list_free(&users);
    list_free(&msgs);
    return 0;
}