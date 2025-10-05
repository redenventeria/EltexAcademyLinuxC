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
#include<signal.h>

#include "common.h"

#define MAX_CLIENTS 100

int sock;

client_id clients[MAX_CLIENTS] = {0};
size_t clients_count[MAX_CLIENTS] = {0};
size_t clients_n = 0;

void get_client_id(client_id *id, const echo_request_ip *ip,  const struct sockaddr *client) {
    id->ip = ((struct sockaddr_in*)client)->sin_addr.s_addr;
    id->port = ((echo_request*)ip->buf)->udp.source;
}

int comp_client_id(const client_id *a, const client_id *b) {
    return a->ip == b->ip && a->port == b->port;
}

ssize_t find_client(client_id *client)
{
    size_t i = 0;
    while(i < clients_n && !comp_client_id(client, &clients[i]))
        i++;

    if(i == clients_n)
        return -1;
    
    return i;
}

void add_new_client(client_id client) {

    if(clients_n == MAX_CLIENTS) {
        printf("Cannot accept new client!\n");
        return;
    }

    memcpy(&clients[clients_n], &client, sizeof(client));
    clients_count[clients_n] = 0;
    clients_n++;
}

void disconnect_client(int i) {

    for(size_t k = i; k < clients_n; k++) {
        clients[k] = clients[k + 1];
    }

    memset(&clients[clients_n - 1], 0, sizeof(clients[clients_n - 1]));
    clients_count[clients_n - 1] = 0;

    clients_n--;
}

void construct_response(const echo_request_ip *request, echo_response *response, int i) {

    echo_request *echo_req = (echo_request*)request->buf;
    response->udp.check = 0;
    response->udp.dest = echo_req->udp.source; // TODO: MAKE PORT INFO TRANSFER CORRECT!
    response->udp.source = htons(SERVER_PORT);
    response->udp.len = htons(sizeof(echo_response));

    //printf("data pos is %d\n", (int)(response->data - (char*)response));
    memcpy(response->data, echo_req->data, sizeof(response->data));
    response->ord = clients_count[i];
}

void response_client(int i, const echo_request_ip *request, const struct sockaddr *client) {
        
        clients_count[i]++;

        echo_response response = {0};
        construct_response(request, &response, i);

        if(sendto(
            sock,
            &response,
            sizeof(response),
            0,
            client,
            sizeof(*client))
        == -1)
            perror("sendto");

        printf("Sent back %s %lu\n", response.data, response.ord);

}

void close_socket(__attribute__((unused)) int _) {
    printf("\n");

    if(close(sock) == -1)
        perror("close");
    
    printf("Resources freed successfully.\n");

    exit(EXIT_SUCCESS);
}

int main()
{

    struct sigaction act = { 0 }, oldact;
    act.sa_handler = &close_socket;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGINT, &act, &oldact) == -1)
        perror("sigaction");

    printf("Press ctrl+c to close server\n");

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(ADDR);
    server.sin_port = 0;

    if(bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    while(1) {

        struct sockaddr client = {0};
        socklen_t client_size = sizeof(client);
        echo_request_ip request = {0};
        if (recvfrom(
            sock,
            &request,
            sizeof(request),
            0,
            &client,
            &client_size
        ) == -1)
            perror("recvfrom");

        echo_request *echo_req = (echo_request*)request.buf;
        //printf("dest: %d %d %d\n", request.udp.dest, SERVER_PORT);
        if(ntohs(echo_req->udp.dest) != SERVER_PORT)
            continue;
        
        printf("Received %s\n", echo_req->data);
        

        client_id id;
        get_client_id(&id, &request, &client);
        ssize_t i = find_client(&id);

        if(i == -1) {
            printf("New client with port %d!\n", echo_req->udp.source);
            add_new_client(id);
            i = clients_n - 1;
        }

        if(!strcmp(echo_req->data, "exit")) {
            disconnect_client(i);
            printf("Client disconnected\n");
        }
        else {
            response_client(i, &request, &client);
        }
    }

    return 0;
}