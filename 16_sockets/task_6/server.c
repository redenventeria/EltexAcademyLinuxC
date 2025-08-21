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
    server.sin_addr.s_addr = htonl(ADDR);
    server.sin_port = htons(SERVER_PORT);

    if(bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    char request[256];
    if(recvfrom(
        sock,
        &request,
        sizeof(request),
        0,
        NULL,
        0
    ) == -1) {
        perror("Unable to recv message!");
    }
    else {
        printf("Received %s from client.\n", request);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return EXIT_SUCCESS;
    

}