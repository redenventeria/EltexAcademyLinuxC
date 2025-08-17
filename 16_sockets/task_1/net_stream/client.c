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
    int sock;
    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;
    
    if(connect(sock, (struct sockaddr*)&server, sizeof(server))) {
        perror("Unable to connect!");
        exit(EXIT_FAILURE);
    }
    
    
    char ans[256];
    if(recv(sock, &ans, sizeof(ans), 0) == -1) {
        perror("Unable to recv message");
    }

    char msg[] = "Hi!";
    if(send(sock, msg,sizeof(msg), 0) == -1) {
        perror("Message didnt sent!");
    }

    shutdown(sock, SHUT_RDWR);

    printf("Received %s form server\n", ans);
    

}