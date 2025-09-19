#ifndef __COMMON_H__
#define __COMMON_H__

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define SERVER_CONNECT_KEY 1

#define CLIENT_INIT 1
#define CLIENT_MSG 2
#define CLIENT_CLOSE 3
#define SERVER_NEWUSR 4
#define SERVER_NEWMSG 5
#define SERVER_DISCONNECT_USR 3

#define MAX_MESSAGES 100
#define MAX_USERS 100

#define ENTRY_LEN 256

#define USER 1
#define MESSAGE 2

struct message {
    char name[ENTRY_LEN];
    char text[ENTRY_LEN];
};

struct msgbuf {
    long mtype;
    char data[2048];
};

struct connect_msg {
    int clientid;
    char name[16];
};

void print_msg(void *a);
void print_usr(void *a);
void print_str(void *a);

int comp_msg(void *a1, void *b1);
int comp_con(void *a1, void *b1);
int comp_str(void *a1, void *a2);

#endif