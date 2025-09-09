#ifndef __CLIENT_UI_H__
#define __CLINET_UI_H__

#include<curses.h>
#include"list.h"
#include"common.h"

struct win {
    WINDOW *w;
    struct list log;
    int x_size, y_size;
    int n;
};

struct input {
    WINDOW *w;
    int x_size, y_size;
};

/*void init_win(
    struct win *w,
    int x_size,
    int y_size
);

void init_input(
    struct input *w,
    int x_size,
    int y_size
);*/

void UI_init(
    struct win *msgs,
    struct win *users,
    struct input *in
);

void add_text(struct win *w, struct input *in, char *message);
void remove_text(struct win *w, struct input *in, char *message);
void win_refresh_full(struct win *w, struct input *in);
void construct_text(char *dest, struct message *a);
void UI_mainloop(struct win *users, struct win *msgs, struct input *in, int ui_mq);

#endif