#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdint.h>

#include"common.h"

void sendraw(int sock, struct sockaddr_in server, socklen_t serv_size) {

    char msg[] = "Hello from raw socket!";
    int msg_size = sizeof(msg);
    char data[8 + sizeof(msg)];
    data[0] = ((htons(CLIENT_PORT) & 0xFF));
    data[1] = ((htons(CLIENT_PORT) & 0xFF00) >> 8);
    data[2] = ((htons(SERVER_PORT) & 0xFF));
    data[3] = ((htons(SERVER_PORT) & 0xFF00) >> 8);
    data[4] = ((htons(sizeof(data)) & 0xFF));
    data[5] = ((htons(sizeof(data)) & 0xFF00) >> 8);
    data[6] = 0;
    data[7] = 0;
    strncpy(data + 8, msg, msg_size);

    if(sendto(
        sock,
        data,
        sizeof(data),
        0,
        (struct sockaddr *)&server,
        serv_size
    ) != sizeof(data)) {
        perror("Message didnt sent!");
    }
}

void recvraw(int sock, struct sockaddr_in server, socklen_t serv_size) {

    int received = 0;
    char ans[256];
    while(!received) {
        memset(ans, 0, sizeof(ans));
        if(recvfrom(
            sock,
            &ans,
            sizeof(ans), 
            0,
            (struct sockaddr *)&server,
            &serv_size
        ) == -1) {
            perror("Unable to recvfrom message");
        }

        uint16_t sender = ((uint16_t)ans[20] & 0xff) | 
                         (((uint16_t)ans[21] & 0xff) << 8);
        sender = ntohs(sender);

        if(sender == SERVER_PORT)
            received = 1;
    }

    printf("Received %s from server\n", ans+28);
}

int main()
{
    if (access("socket1", F_OK) == 0) {
        unlink("socket1");
        printf("Socket removed\n");
    }

    int sock;
    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    //int a = 1;
    //int len = sizeof(a);
    //if(setsockopt(sock, SOL_IP, IP_HDRINCL, &a, len))
    //    perror("setsockopt");
    //printf("%d\n", a);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(ADDR);
    server.sin_port = 0;
    socklen_t serv_size = sizeof(server);

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(ADDR);
    client.sin_port = 0;
    socklen_t cli_size = sizeof(client);

    if(bind(sock, (struct sockaddr*)&client, cli_size)) {
        perror("Unable to bind!");
    }

    //
    //if(connect(sock, (struct sockaddr*)&server, sizeof(server))) {
    //    perror("Unable to connect!");
    //    exit(EXIT_FAILURE);
    //} 
    
    sendraw(sock, server, serv_size);

    recvraw(sock, server, serv_size); 

    //shutdown(sock, SHUT_RDWR);

    if (access("socket1", F_OK) == 0) {
        unlink("socket1");
        printf("Socket removed\n");
    }
    return EXIT_SUCCESS;

}