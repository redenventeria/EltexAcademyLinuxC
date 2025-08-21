#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#include<sys/socket.h>
#include<netinet/in.h>

#include<sys/select.h>
#include<sys/wait.h>
#include<stdatomic.h>

#include"common.h"

void errExit(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int create_socket(int address, int port, int type)
{
    int sock = socket(AF_INET, type, 0);
    if(sock == -1)
        errExit("sock");
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(address);
    if(bind(
        sock,
        (struct sockaddr *)&server,
        sizeof(server)
    )) errExit("bind");

    return sock;
}

void worker_tcp(int sock) {
    struct sockaddr_in clientaddr;
    unsigned int clientaddr_size = sizeof(clientaddr);
    int clientfd = accept(
        sock, 
        (struct sockaddr *)&clientaddr, 
        &clientaddr_size);
    if(clientfd == -1) {
        perror("connect");
        return;
    }

    char ans[256];
    if(recv(clientfd, ans, sizeof(ans), 0) == -1)
        perror("recv");
    
    printf("Received %s\n", ans);

    char msg[] = "Hello!";
    if(send(clientfd, msg, sizeof(msg), 0) == -1)
        perror("send");
    
    shutdown(clientfd, SHUT_RDWR);
    close(clientfd);
}

void worker_udp(int sock) {
    struct sockaddr_in clientaddr;
    unsigned int clientaddr_size = sizeof(clientaddr);

    char ans[256];
    if(recvfrom(
        sock,
        ans,
        sizeof(ans),
        0,
        (struct sockaddr*)&clientaddr,
        &clientaddr_size
    ) == -1) perror("recv");

    printf("Received %s\n", ans);

    char msg[] = "Hello!";
    if(sendto(
        sock,
        msg,
        sizeof(msg),
        0,
        (struct sockaddr*)&clientaddr,
        clientaddr_size
    ) == -1)
        perror("send");

}

void handle_tcp(int sock) {
    int childpid = fork();
    if(childpid == 0) {
        worker_tcp(sock);
        exit(EXIT_SUCCESS);
    }
}

void handle_udp(int sock) {
    int childpid = fork();
    if(childpid == 0) {
        worker_udp(sock);
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    int sock_tcp = create_socket(ADDR, SERVER_PORT, SOCK_STREAM);
    int sock_udp = create_socket(ADDR, SERVER_PORT, SOCK_DGRAM);

    if(listen(sock_tcp, 100) == -1)
        errExit("listen");
    
    fd_set duplex_read;
    FD_ZERO(&duplex_read);
    FD_SET(sock_tcp, &duplex_read);
    FD_SET(sock_udp, &duplex_read);
    int max = sock_tcp > sock_udp ? sock_tcp : sock_udp;
    max++;

    struct timeval t;
    t.tv_sec = 5;
    t.tv_usec = 0;

    printf("Server timeout is 5 seconds\n");

    int count = 0;

    while(1) {
        
        int n;
        n = select(
            max,
            &duplex_read,
            NULL,
            NULL,
            &t
        );
        if(n == -1) perror("select");

        if(n == 0) {
            printf("No more connections, exititng...\n");
            break;
        }

        if(FD_ISSET(sock_tcp, &duplex_read)) {
            handle_tcp(sock_tcp);
            count++;
        }
        if(FD_ISSET(sock_udp, &duplex_read)) {
            handle_udp(sock_udp);
            count++;
        }
        
    }

    int stat;
    while(wait(&stat) > 0) {};
    printf("Total requests: %d\n", count);

    shutdown(sock_tcp, SHUT_RDWR);
    shutdown(sock_udp, SHUT_RDWR);
    close(sock_tcp);
    close(sock_udp);
}