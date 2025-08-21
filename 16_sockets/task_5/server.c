#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#include "common.h"

void uppercaseify(char *s) {
    for(size_t i = 0; s[i] != '\0'; i++) {
        if(97 <= s[i] && s[i] <= 122)
            s[i] -= 32;
    }
}

int main()
{
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(SERVER_PORT);

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
    char request[256];
    if(recvfrom(
        sock,
        &request,
        sizeof(request),
        0,
        (struct sockaddr*)client,
        &size
    ) == -1) {
        perror("Unable to recv message!");
    }
    else {
        printf("Received %s from client.\n", request);
    }

    uppercaseify(request);

    char append[] = "Uppercased: ";
    size_t append_len = strnlen(append, sizeof(append));
    size_t request_len = strnlen(request, sizeof(request));
    char response[append_len + request_len + 1];
    strncpy(response, append, append_len);
    strncpy(response + append_len, request, request_len);
    response[sizeof(response) - 1] = '\0';

    if(sendto(
        sock,
        response,
        sizeof(response),
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

    return EXIT_SUCCESS;
    

}