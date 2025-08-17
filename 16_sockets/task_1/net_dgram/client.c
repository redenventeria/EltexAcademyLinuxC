#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    if (access("socket1", F_OK) == 0) {
        unlink("socket1");
        printf("Socket removed\n");
    }

    int sock;
    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;

    client.sin_family = AF_INET;
    socklen_t size = sizeof(client);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    client.sin_port = 7778;
    
    if(bind(sock, (struct sockaddr*)&client, size)) {
        perror("Unable to bind!");
    }
    
    if(connect(sock, (struct sockaddr*)&server, sizeof(server))) {
        perror("Unable to connect!");
        exit(EXIT_FAILURE);
    }
    
    char msg[] = "Hi!";
    if(send(
        sock,
        msg,
        sizeof(msg),
        0
    ) == -1) {
        perror("Message didnt sent!");
    }
    
    char ans[256];
    if(recv(
        sock,
        &ans,
        sizeof(ans), 
        0
    ) == -1) {
        perror("Unable to recvfrom message");
    }

    //shutdown(sock, SHUT_RDWR);

    printf("Received %s form server\n", ans);

    if (access("socket1", F_OK) == 0) {
        unlink("socket1");
        printf("Socket removed\n");
    }
    return EXIT_SUCCESS;

}