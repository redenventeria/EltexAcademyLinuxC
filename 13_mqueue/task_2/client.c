#include<sys/types.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>


#include"client_IPC.h"
#include"client_UI.h"
#include"common.h"
#include"list.h"

struct list msgs;
struct list users;
int clientid;
int serverid;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Usage: client <username>");
        return 0;
    }

    client_init_connection(
        SERVER_CONNECT_KEY,
        argv[1],
        &clientid,
        &serverid
    );
    list_init(&msgs, sizeof(struct message));
    list_init(&users, sizeof(struct connect_msg));

    int ui_mq = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);

    struct win users_UI;
    struct win msgs_UI;
    struct input in_UI;

    UI_init(&msgs_UI, &users_UI, &in_UI);

    pid_t a = fork();
    if(a == 0) {
        char msg[256];
        
        while(1) {
            //sleep(1);
            werase(in_UI.w);
            wmove(in_UI.w, 0, 0);
            wrefresh(in_UI.w);
            wgetstr(in_UI.w, msg);
            if(!strcmp(msg, "exit")) {
                client_close_connection(clientid, argv[1], serverid);
                msgctl(ui_mq, IPC_RMID, NULL);
                break;
            }
            //sleep(1);
            send(serverid, argv[1], msg);
        }

        return 0;
    }

    a = fork();
    //printf("a is %d\n", a);
    if(a == 0) {
        client_listener(clientid, &msgs, &users, ui_mq);
        return 0;
    }

    UI_mainloop(&msgs_UI, &users_UI, &in_UI, ui_mq);

    int stat;
    while (wait(&stat) != -1) {}
    list_free(&msgs);
    list_free(&users);

    endwin();

    return EXIT_SUCCESS;
}