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
    int addr = INADDR_ANY;
    int sock = create_server_sock(addr, 7777);
    
    struct ip_mreqn mcast_addr;
    mcast_addr.imr_address.s_addr = addr;
    mcast_addr.imr_multiaddr.s_addr = addr;
    mcast_addr.imr_ifindex = 1;

    struct sockaddr_in multicast;
    multicast.sin_family = AF_INET;
    multicast.sin_addr.s_addr = addr;
    multicast.sin_port = 9999;
    if(setsockopt(
        sock,
        SOL_SOCKET,
        IP_MULTICAST_IF,
        &mcast_addr,
        sizeof(mcast_addr)
    )) perror("setsockopt");

    int i = 5;
    while(i > 0) {
        printf("Multicasting in %d\n", i);
        sleep(1);
        i--;
    }

    char msg[] = "Hello!";
    if(sendto(
        sock,
        msg,
        sizeof(msg), 
        0,
        (struct sockaddr *) &multicast,
        sizeof(multicast)
    ) != -1)
        perror("sendto");
    
    if(close(sock))
        perror("close");

    return EXIT_SUCCESS;
}