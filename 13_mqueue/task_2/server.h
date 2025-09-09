#ifndef __SERVER_H__
#define __SERVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/msg.h>

#include"common.h"

int init_msgqueue();

void server_listener(int id);

void accept_con(struct connect_msg *a);
void receive(struct message *a);
void close_con(struct connect_msg *a);
int send_chat_info(struct connect_msg *a);
int inform_usr(int clientid, struct connect_msg *a, int type);
int inform_msg(int clientid, struct message *a);
int broadcast_new_user(struct connect_msg *a, int type);
int broadcast_new_msg(struct message *a);

#endif