#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#include<sys/stat.h>

#include<sys/msg.h>
#include<errno.h>

#include"client_IPC.h"
#include"common.h"
#include"list.h"

void client_listener(int clientid, struct list *msgs, struct list *users, int ui_id) {
    while(1) {

        struct msgbuf a;
        if(msgrcv(clientid, &a, sizeof(a.data), 0, 0) == -1) {
            if(errno != EIDRM)
                perror("handler msgrcv");
            return;
        }

        //print_usr(a.data);

        if(msgsnd(ui_id, &a, sizeof(a.data), 0) == -1)
            perror("UI msgsnd");

        // TODO: Провести очередь сообщений процессами и передавать информацию о всех событиях на UI.

        switch(a.mtype) {
            case SERVER_NEWMSG:
                //printf("Received new message!\n");
                list_add(msgs, a.data);
                //print(msgs, print_msg);
                break;
            case SERVER_NEWUSR:
                //printf("Received new user!\n");
                list_add(users, a.data);
                //print(users, print_usr);
                break;
            case SERVER_DISCONNECT_USR:
                //printf("Disconnecting user!\n");
                list_del(users, a.data, comp_con);
                //print(users, print_usr);
                break;
            default: {}
                //printf("Unreachable\n");
        }

    }
}

void send(int serverid, char* author, char *msg_text) {
    struct msgbuf b;
    b.mtype = CLIENT_MSG;
    struct message *msg = (struct message*)b.data;
    strncpy(msg->name, author, sizeof(msg->name));
    strncpy(msg->text, msg_text, sizeof(msg->text));

    if(msgsnd(serverid, &b, sizeof(b.data), 0)) {
        perror("msgsnd");
    }
    else {}
        //printf("message sent!\n");


}
 
int client_init_connection(
    key_t key,
    const char *cli_name,
    int *clientid,
    int *serverid
) {

    *serverid = msgget(key, S_IWUSR | S_IRUSR);
    if(*serverid == -1) {
        perror("msgget serverid");
        exit(EXIT_FAILURE);
    }

    *clientid = msgget(IPC_PRIVATE, S_IWUSR | S_IRUSR);
    if(*clientid == -1) {
        perror("msgget clientid");
        exit(EXIT_FAILURE);
    }

    struct connect_msg cli_info;
    cli_info.clientid = *clientid;
    strncpy(cli_info.name, cli_name, sizeof(cli_info.name));
    struct msgbuf msg;
    msg.mtype = CLIENT_INIT;
    memcpy(msg.data, &cli_info, sizeof(cli_info));

    if(msgsnd(*serverid, &msg, sizeof(cli_info), 0) == -1) {
        perror("msgsnd init");
    }

    return 0;
}


void client_close_connection(int clientid, char *name, int serverid) {

    struct msgbuf a;
    a.mtype = CLIENT_CLOSE;
    struct connect_msg *b = (struct connect_msg*)&a.data;
    b->clientid = 0;
    strncpy(b->name, name, sizeof(b->name));
    if(msgsnd(serverid, &a, sizeof(a.data), 0)) {
        perror("msgsnd closing connection");
    }

    msgctl(clientid, IPC_RMID, NULL);
}