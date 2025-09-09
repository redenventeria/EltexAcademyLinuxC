#ifndef __CLIENT_IPC_H__
#define __CLIENT_IPC_H__

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

#include"list.h"

void client_listener(int clientid, struct list *msgs, struct list *users, int ui_id);
 
int client_init_connection(
    key_t key,
    const char *cli_name,
    int *clientid,
    int *serverid
);

void send(int serverid, char* author, char *msg_text);

int get_chat_info(int clientid, struct list *msgs, struct list *users);

void client_close_connection(int clientid, char* name, int serverid);

#endif