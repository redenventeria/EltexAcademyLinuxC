#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/wait.h>

#define B_PORT 9999


int create_server_sock(int addr, int port)
{
    int sock;

    struct sockaddr_in server;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = addr;
    server.sin_port = port;
    if(bind(sock, (struct sockaddr *) &server, sizeof(server))) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main()
{
    printf("%d\n", getpid());
    int addr = inet_addr("127.0.0.1");
    int sock = create_server_sock(addr, 7777);
    int broadcast_accept = 1;
    setsockopt(
        sock,
        SOL_SOCKET,
        SO_BROADCAST,
        &broadcast_accept,
        sizeof(broadcast_accept)
    );

    struct sockaddr_in broadcast;
    broadcast.sin_family = AF_INET;
    broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast.sin_port = B_PORT;

    int i = 5;
    while(i > 0) {
        printf("Broadcasting in %d\n", i);
        sleep(1);
        i--;
    }

    char msg[] = "Hello!";
    if(sendto(
        sock,
        msg,
        sizeof(msg), 
        0,
        (struct sockaddr *) &broadcast,
        sizeof(broadcast)
    ))
        perror("sendto");
    
    if(close(sock))
        perror("close");

    return EXIT_SUCCESS;
}