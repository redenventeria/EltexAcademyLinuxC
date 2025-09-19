#include<curses.h>
#include<sys/ioctl.h>
#include<sys/msg.h>
#include<string.h>
#include<errno.h>

#include"client_UI.h"
#include"common.h"
#include <ncurses.h>

#define USERS_WIDTH 16
#define INPUT_HEIGHT 1

// TODO: Разобраться, какого черта программа ничего не выводит на экран!

void UI_init(struct win *msgs, struct win *users, struct input *in)
{
    initscr();

    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);

    msgs->x_size = size.ws_col - USERS_WIDTH;
    msgs->y_size = size.ws_row - INPUT_HEIGHT;
    msgs->w = newwin(
        msgs->y_size,
        msgs->x_size,
        0,
        0
    );

    users->x_size = USERS_WIDTH;
    users->y_size = size.ws_row - INPUT_HEIGHT;
    users->w = newwin(
        users->y_size,
        users->x_size,
        0,//msgs->x_size,
        msgs->x_size
    );

    in->x_size = size.ws_col;
    in->y_size = INPUT_HEIGHT;
    in->w = newwin(
        in->y_size,
        in->x_size,
        size.ws_row - INPUT_HEIGHT,
        0
    );
    if(in->w == NULL)
        perror("newwin");

    users->type = USER;
    msgs->type = MESSAGE;

    //printf("%d %d %d %d\n", msgs->y_size, msgs->x_size, 0, 0);
    //printf("%d %d %d %d\n", users->y_size, users->x_size, 0, msgs->x_size);
    //printf("%d %d %d %d\n", in->y_size, in->x_size, size.ws_row - INPUT_HEIGHT, 0);
    //printf("%d %d\n", size.ws_col, size.ws_row);
    //sleep(10);

    //for(int a = 0; a < 21999; a++){
    //    waddch(msgs->w, 'A');
    //}
    //for(int a = 0; a < 1999; a++){
    //    waddch(users->w, 'B');
    //}
    //for(int a = 0; a < 1999; a++){
    //    waddch(in->w, 'C');
    //}

    //waddstr(msgs->w, "AAA: Hello!\n");
    //waddstr(msgs->w, "BBB: Hi!\n");
    //waddstr(users->w, "AAA\n");
    //waddstr(users->w, "BBB\n");

    wrefresh(msgs->w);
    wrefresh(users->w);
    wmove(in->w, 0, 0);
    wrefresh(in->w);
}

/*void add_text(struct win *w, struct input *in, char *message)
{
    list_add(&w->log, message);
    waddstr(w->w, message);
    waddch(w->w, '\n');
    wrefresh(w->w);
    
    wmove(in->w, 0, 0);
    wrefresh(in->w);
}*/

/*void rm_text(struct win *w, struct input *in, char *message)
{
    list_del(&w->log, message, comp_str);

    win_refresh_full(w, in);
}*/

void win_refresh_full(struct win *w, struct input *in)
{
    werase(w->w);
    wrefresh(w->w);
    wmove(w->w, 0, 0);

    if(w->type == USER) {
        struct connect_msg *usrs = w->data;
        int i = 1;
        while(usrs[i].clientid != 0) {
            waddstr(w->w, usrs[i].name);
            waddch(w->w, '\n');
            i++;
        }
    }
    else {
        struct message *msgs = w->data;
        int i = 1;
        while(msgs[i].name[0] != 0) {
            waddstr(w->w, msgs[i].name);
            waddstr(w->w, ": ");
            waddstr(w->w, msgs[i].text);
            waddch(w->w, '\n');
            i++;
        }
    }

    /*for(size_t i = 0; i < w->log.n; i++) {
        waddstr(w->w, ((char*)w->log.data[i]));
        waddch(w->w, '\n');
    }*/
    wrefresh(w->w);

    wmove(in->w, 0, 0);
    wrefresh(in->w);
}

/*void UI_mainloop(struct win *msgs, struct win *users, struct input *in, int ui_mq)
{
    while(1) {
        struct msgbuf a;
        if(msgrcv(ui_mq, &a, sizeof(a), 0, 0) == -1) {
            if(errno != EIDRM)
                perror("UI msgrcv");
            return;
        }

        struct message *b = (struct message*)a.data;
        struct connect_msg *c = (struct connect_msg*)a.data;
        char text[256];

        switch(a.mtype) {
            case SERVER_NEWMSG:
                //printf("Received new message!\n");
                construct_text(text, b);
                //printf("Constructed %s\n", text);
                add_text(msgs, in, text);
                break;
            case SERVER_NEWUSR:
                add_text(users, in, c->name);
                break;
            case SERVER_DISCONNECT_USR:
                rm_text(users, in, c->name);
                break;
            default:
                printf("Unreachable\n");
        }
    }
}*/

/*void construct_text(char *dest, struct message *a) {
    dest[0] = '\0';
    strcat(dest, a->name);
    strcat(dest, ": ");
    strcat(dest, a->text);
}*/



/*int main()
{
    struct win msgs;
    struct win users;
    struct input in;
    
    UI_init(&msgs, &users, &in);

    char msg[256];
    while(1) {
        wclear(in.w);
        wmove(in.w, 0, 0);
        wgetstr(in.w, msg);
        waddstr(msgs.w, msg);
        waddch(msgs.w, '\n');
        wrefresh(msgs.w);
    }

    delwin(msgs.w);
    delwin(users.w);
    delwin(in.w);

    endwin();
}*/