#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<malloc.h>
#include<errno.h>

#define BUF 1024

void exec_pipe(char **call)
{
    size_t i = 0;
    int pipe_loc = -1;
    while(call[i] != NULL) {
        if(!strncmp(call[i], "|", BUF))
            pipe_loc = i;
        i++;
    }

    if(pipe_loc != -1) {
        call[pipe_loc] = NULL;
        int fd[2];

        pid_t p1 = fork();
        if(p1 == 0)
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            int err = execvp(call[0], call);
            if(err == -1) {
                perror("Error! unable to execute command 1!\n");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
        else if (p1 == -1) {
            perror("Unable to execute child!");
        }
        else {
            printf("Waiting...\n");
            int stat;
            waitpid(p1, &stat, 0);
        }

        char eof = EOF;
        write(fd[1], &eof, 1);

        pid_t p2 = fork();
        if(p2 == 0)
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            int err = execv(call[pipe_loc + 1], call + pipe_loc + 1);
            if(err == -1) {
                perror("Error! unable to execute command 2!\n");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
        else if (p2 == -1) {
            perror("Unable to execute child 2!");
        }
        else {
            int stat;
            waitpid(p2, &stat, 0);
        }

        close(fd[0]);
        close(fd[1]);

    }
    else {
        pid_t a = fork();
        int err = 0;
        if(a == 0) {
            err = execv(call[0], call);
            if(err == -1) {
                perror("Error! unable to execute command!\n");
                exit(EXIT_FAILURE);
            }
        }
        else {
            int stat;
            waitpid(a, &stat, 0);
        }
    }

    // /bin/ls | /bin/grep task

}

int main()
{
    char *command = calloc(BUF, sizeof(char));
    char *running = NULL;
    char *og_running = NULL;
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
        og_running = running; // Saving pointer to free memory

        int n = 0;
        char *token = strsep(&running, " ");
        while(token != NULL) {
            call[n] = token;
            token = strsep(&running, " ");
            n++;
        }
        call[n] = NULL;



        if(strncmp(command, "exit", 6)) {
            exec_pipe(call);
        }

        free(og_running);

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