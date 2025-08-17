#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in server;

    if (access("socket", F_OK) == 0) {
        unlink("socket");
        printf("Socket removed\n");
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;

    if(bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if(listen(sock, 0)) {
        perror("Unable to make socket passive");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    socklen_t size = sizeof(client);
    int cli_fd = accept(sock, (struct sockaddr *)&client, &size);
    if(cli_fd == -1) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }

    char msg[] = "Hello!";
    if(send(cli_fd, msg, sizeof(msg), 0) != sizeof(msg)) {
        perror("Message didnt sent!");
    }
    char ans[256];
    if(recv(cli_fd, &ans, sizeof(ans), 0) == -1) {
        perror("Unable to recv message!");
    }

    printf("Received %s from client\n", ans);

    shutdown(sock, SHUT_RDWR);

    if (access("socket", F_OK) == 0) {
        unlink("socket");
        printf("Socket removed\n");
    }
    

}