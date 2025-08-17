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

    sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    /*
    if(listen(sock, 0)) {
        perror("Unable to make socket passive");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un client;
    socklen_t size = sizeof(client);
    int cli_fd = accept(sock, (struct sockaddr *)&client, &size);
    if(cli_fd == -1) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }
    */

    struct sockaddr_un *client = malloc(sizeof(struct sockaddr_in));
    socklen_t size = sizeof(struct sockaddr_in);
    char ans[256];
    if(recvfrom(
        sock,
        &ans,
        sizeof(ans),
        0,
        (struct sockaddr*)client,
        &size
    ) == -1) {
        perror("Unable to recv message!");
    }
    else {
        printf("Received %s from client.\n", ans);
    }

    char msg[] = "Hello!";
    if(sendto(
        sock,
        msg,
        sizeof(msg),
        0, 
        (struct sockaddr *)client,
        size
    ) == -1) {
        perror("Message didnt sent!");
    }
    if(sendto(
        sock,
        msg,
        sizeof(msg),
        0, 
        (struct sockaddr *)client,
        size
    ) == -1) {
        perror("Message didnt sent!");
    }

    //shutdown(sock, SHUT_RDWR);

    if (access("socket", F_OK) == 0) {
        unlink("socket");
        printf("Socket removed\n");
    }
    

}