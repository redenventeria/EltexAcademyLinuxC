#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>

int main()
{

    if(mkfifo("my_pipe", S_IRUSR | S_IWUSR)) {
        perror("Unable to create pipe");
        return EXIT_FAILURE;
    }

    FILE *f = fopen("my_pipe", "w");
    if(f == NULL) {
        perror("Unable to open pipe");
        return EXIT_FAILURE;
    }

    char msg[] = "Hi!";
    printf("Server sent \"%s\" to named pipe\n", msg);
    fprintf(f, "%s", msg);

    fclose(f);

    if(unlink("my_pipe")) {
        perror("Unable to delete pipe");
        return EXIT_FAILURE;
    }
    
    return 0;
}