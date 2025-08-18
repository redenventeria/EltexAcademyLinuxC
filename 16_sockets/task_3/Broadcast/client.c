#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>

#define CLIENT_N 10
#define B_PORT 9999

void client()
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    int allow_reuseaddr = 1;
    if(setsockopt(
        sock,
        SOL_SOCKET,
        SO_REUSEPORT,
        &allow_reuseaddr,
        sizeof(allow_reuseaddr)
    )) perror("sotsockopt");
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;
    unsigned int serv_size = sizeof(server);

    struct sockaddr_in broadcast;
    broadcast.sin_family = AF_INET;
    broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast.sin_port = B_PORT;
    if(bind(
        sock,
        (struct sockaddr *)&broadcast,
        sizeof(broadcast)
    )) perror("bind");

    //client.sin_family = AF_INET;
    //client.sin_addr.s_addr = inet_addr("127.0.0.1");
    //client.sin_port = 0;

    //bind(sock, (struct sockaddr *)&client, sizeof(client));
    
    
    
    char ans[256];
    printf("Waiting...\n");
    if(recvfrom(
        sock,
        &ans,
        sizeof(ans),
        0,
        (struct sockaddr*)&server,
        &serv_size
    ) == -1) {
        perror("Unable to recv message");
        exit(EXIT_FAILURE);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    printf("Received %s form server\n", ans);
    
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