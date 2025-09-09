#ifndef __LIST_H__
#define __LIST_H__

#include<stddef.h>
#include<pthread.h>

#define MAX_LIST_LEN 1000

typedef struct list {
    size_t n;
    size_t elem_size;
    void *data[MAX_LIST_LEN];
    pthread_mutex_t lock;
} list;

int list_init(list *l, size_t elem_size);
int list_add(list *l, void *elem);
int list_del(list *l, void *elem, int (*comp)(void*, void*));
void list_free(list *l);

void print(list *l, void (*elem_print)(void*));

#endif