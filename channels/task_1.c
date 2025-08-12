#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

int main()
{
    pid_t a;

    int fd[2];
    if(pipe(fd)) {
        perror("Error, failed to create pipe!");
        return EXIT_FAILURE;
    }

    a = fork();
    if(a == -1) {
        perror("Unable to create child, exiting");
        return EXIT_FAILURE;
    }

    if(a == 0) {
        close(fd[1]);

        FILE *f = fdopen(fd[0], "r");
        if(f == NULL) {
            perror("Child process unable to open pipe");
            return EXIT_FAILURE;
        }

        char *s = NULL;
        size_t n = 255;
        if(getline(&s, &n, f) == -1) {
            perror("Unable to read from stream, exiting");
            return EXIT_FAILURE;
        }
        
        printf("Child process recieved message \"%s\"\n", s);

        free(s);
        if(fclose(f)) {
            perror("Unable to close file");
            return EXIT_FAILURE;
        }
        close(fd[0]);
    }
    else {
        close(fd[0]);
        
        FILE *f = fdopen(fd[1], "w");
        if(f == NULL) {
            perror("Parent process unable to open pipe");
            return EXIT_FAILURE;
        }

        char msg[] = "Hi!";
        fprintf(f, "%s", msg);
        printf("Parent process sent \"Hi!\" to pipe\n");
        int stat;
        
        if(fclose(f)) {
            perror("Unable to close file");
            return EXIT_FAILURE;
        }
        close(fd[1]);

        waitpid(a, &stat, 0);
    }
    return EXIT_SUCCESS;
}