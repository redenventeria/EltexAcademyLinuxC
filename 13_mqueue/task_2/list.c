#include<errno.h>
#include<string.h>
#include<stddef.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

#include"list.h"

int list_init(list *l, size_t elem_size) {
    l->n = 0;
    pthread_mutex_init(&l->lock, NULL);
    l->elem_size = elem_size;
    return 0;
}

int list_add(list *l, void *elem) {

    pthread_mutex_lock(&l->lock);
    if(l->n >= MAX_LIST_LEN) {
        errno = ENOBUFS;
        return -1;
    }

    l->data[l->n] = malloc(l->elem_size);
    memcpy(l->data[l->n], elem, l->elem_size);
    l->n++;

    pthread_mutex_unlock(&l->lock);
    return 0;
}

int list_del(list *l, void *elem, int (*comp)(void*, void*)) {

    pthread_mutex_lock(&l->lock);
    
    size_t i, found = 0;
    for(i = 0; i < l->n; i++) {
        if(comp(l->data[i], elem)) {
            found = 1;
            break;
        }
    }

    if(!found) {
        pthread_mutex_unlock(&l->lock);
        return 1;
    }
    
    free(l->data[i]);

    while(i < l->n - 1) {
        l->data[i] = l->data[i + 1];
        i++;
    }
    l->n--;

    pthread_mutex_unlock(&l->lock);

    return 0;

}

void list_free(list *l) {
    for(size_t i = 0; i < l->n; i++) {
        free(l->data[i]);
    }
}

void print(list *l, void (*elem_print)(void*)) {
    printf("Size = %lu\n",l->n);

    for(size_t i = 0; i < l->n; i++)
        elem_print(l->data[i]);
}