#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/un.h>
#include<unistd.h>
#include<errno.h>

int main()
{
    int sock;
    struct sockaddr_un server;

    if (access("socket", F_OK) == 0) {
        unlink("socket");
        printf("Socket removed\n");
    }

    sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, "socket", sizeof (server.sun_path) - 1);

    if(bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_un)) == -1) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un *client = malloc(sizeof(struct sockaddr_un));
    socklen_t size = sizeof(struct sockaddr_un);
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
        printf("Received %s from client %s.\n", ans, client->sun_path);
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

    free(client);
    

}