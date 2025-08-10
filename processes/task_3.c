#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<malloc.h>
#include<errno.h>

#define BUF 1024

int main()
{
    char *command = calloc(BUF, sizeof(char));
    char *running;
    char **call = malloc(sizeof(char*)*BUF);
    int a = 1;
    while(strcmp(command, "exit"))
    {

        printf("[%d] >>> ", a);
        command = memset(command, 0, BUF);
        fgets(command, BUF - 1, stdin);
        char *newline_loc = strchr(command, '\n');
        if(newline_loc != NULL)
            *newline_loc = '\0';
        a++;



        running = strdup(command);

        int n = 0;
        char *token = strsep(&running, " ");
        while(token != NULL)
        {
            call[n] = token;
            token = strsep(&running, " ");
            n++;
        }
        call[n] = NULL;



        int err = 0;
        pid_t a = fork();
        if(a == 0 && strcmp(command, "exit")) {
            int err = execv(call[0], call);
            if(err == -1)
            {
                printf("Error! unable to execute command!\n");
                exit(0);
            }
        }
        else
        {
            int stat;
            waitpid(a, &stat, 0);
        }

        free(running);

        //n = 0;
        //while(call[n] != NULL) {
        //    free(call[n]);
        //    call[n] = NULL;
        //    n++;
        //}
    }

    free(command);
    free(call);
    return 0;
}