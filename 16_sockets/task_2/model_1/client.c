#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/wait.h>

#define CLIENT_N 100

void client()
{
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;

    //client.sin_family = AF_INET;
    //client.sin_addr.s_addr = inet_addr("127.0.0.1");
    //client.sin_port = 0;

    //bind(sock, (struct sockaddr *)&client, sizeof(client));
    
    if(connect(sock, (struct sockaddr*)&server, sizeof(server))) {
        perror("Unable to connect!");
        exit(EXIT_FAILURE);
    }
    
    
    char ans[256];
    if(recv(sock, &ans, sizeof(ans), 0) == -1) {
        perror("Unable to recv message");
        exit(EXIT_FAILURE);
    }

    char msg[] = "Hi!";
    if(send(sock, msg,sizeof(msg), 0) != sizeof(msg)) {
        perror("Message didnt sent!");
        exit(EXIT_FAILURE);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    //printf("Received %s form server\n", ans);
    
}

int proc_c = 0;

int main()
{
    for(int i = 0; i < CLIENT_N; i++)
    {
        int a = fork();
        if(a == 0) {
            client();
            return 0;
        }
        if(a == -1) {
            perror("fork");
            //int stat;
            //while(wait(&stat) > 0) {}
            
            exit(EXIT_FAILURE);
        }
    }

    int stat;
    while(wait(&stat) > 0) {}

    return 0;
}