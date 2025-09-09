#include<stdio.h>
#include"common.h"

void print_msg(void *a) {
    struct message *m = a;
    printf("Author: %s, struct message: %s\n", m->name, m->text);
}

void print_usr(void *a) {
    struct connect_msg *m = a;
    printf("User: %s\n", m->name);
}

void print_str(void *a) {
    char *b = a;
    printf("%s\n", b);
}

int comp_msg(void *a1, void *b1) {
    struct message *a = a1;
    struct message *b = b1;
    return !strcmp(a->name, b->name) && !strcmp(a->text, b->text);
}

int comp_con(void *a1, void *b1) {
    struct connect_msg *a = a1;
    struct connect_msg *b = b1;
    return !strcmp(a->name, b->name);
}

int comp_str(void *a1, void *b1) {
    char *a = a1;
    char *b = b1;
    return !strcmp(a, b);
}