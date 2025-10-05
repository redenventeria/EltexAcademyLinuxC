#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<arpa/inet.h>
#include<string.h>

#include "common.h"

#define MAX_CLIENTS 100

int sock;
int port;

struct sockaddr clients[MAX_CLIENTS] = {0};
size_t clients_count[MAX_CLIENTS] = {0};
size_t clients_n = 0;

void construct_request(echo_request *request, char *s) {
   
    struct udphdr udp = {0};
    udp.check = 0;
    udp.dest = htons(SERVER_PORT);
    udp.source = htons(port);
    udp.len = htons(sizeof(echo_request));

    memcpy(&request->udp, &udp, sizeof(udp));

    size_t cpy_size = sizeof(request->data);
    if(strlen(s) + 1 < cpy_size)
        cpy_size = strlen(s) + 1;
    memcpy(request->data, s, cpy_size);
    /*sizeof(echo_request);
    sizeof(echo_request_ip);
    sizeof(echo_response);
    sizeof(echo_response_ip);*/
}

/*void request_server(int i, echo_request_nopad *request) {
        
        clients_count[i]++;

        echo_response response;
        construct_response(request, &response, i);

        printf("Trying to send %s %lu\n", response.data, response.ord);

        if(sendto(
            sock,
            &response,
            sizeof(response),
            0,
            &clients[i],
            sizeof(clients[i]))
        == -1)
            perror("sendto");

}*/

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: ./client <port>\n");
        exit(EXIT_SUCCESS);
    }

    printf("Type exit to close connection, any other message to receive ping response\n");
    

    struct sockaddr_in client, server;
    //socklen_t client_size = sizeof(client);
    socklen_t server_size = sizeof(server);

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(ADDR);
    client.sin_port = 0;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(ADDR);
    server.sin_port = htonl(SERVER_PORT);

    if(bind(sock, (struct sockaddr*)&client, sizeof(struct sockaddr_in)) == -1) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    //getsockname(sock, (struct sockaddr*)&client, &client_size);
    port = htons(atoi(argv[1]));
    printf("my port is %d\n", ntohs(port));

    char *msg = NULL;
    size_t msg_len = 0;

    while(1) {

        struct sockaddr server = {0};
        echo_request request = {0};

        printf(">");
        fflush(stdout);
        getline(&msg, &msg_len, stdin);
        msg[strlen(msg) - 1] = '\0';


        construct_request(&request, msg);

        if (sendto(
            sock,
            (struct sockaddr*)&request,
            sizeof(request),
            0,
            &server,
            server_size
        ) == -1)
            perror("sendto");
        
        if(!strcmp(msg, "exit"))
            break;

        echo_response_ip response = {0};
        //printf("data pos is %d\n", (int)(response.data - (char*)&response));
        echo_response *echo_resp = (echo_response*)response.buf;
        while(echo_resp->udp.dest != htons(port))
            if (recvfrom(
                sock,
                &response,
                sizeof(response),
                0,
                &server,
                &server_size
            ) == -1)
                perror("sendto");
        
        printf("Received %s %lu\n", echo_resp->data, echo_resp->ord);
        /*char *s = (char*)&response;
        for(size_t i = 0; i < sizeof(response) - 96; i++) {
            printf("%lu: %c %d\n", i, s[i], (uint8_t)s[i]);
            fflush(stdout);
        }
        printf("\n");*/
    }

    if(close(sock) == -1)
        perror("close");
    free(msg);

    printf("Resouces freed successfully.\n");

    return 0;
}