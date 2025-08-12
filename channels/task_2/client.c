#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>

#define BUF_SIZE 256

int main()
{
    FILE *f = fopen("my_pipe", "r");
    if(f == NULL) {
        perror("Unable to open pipe");
        return EXIT_FAILURE;
    }

    char *buf = NULL;
    size_t len = BUF_SIZE;
    getline(&buf, &len, f);
    printf("Client received string \"%s\" from pipe\n", buf);
    free(buf);

    fclose(f);
    
    return 0;
}