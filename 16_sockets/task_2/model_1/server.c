#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/wait.h>


int failure_counter = 0;

int create_server_sock(int addr, int port)
{
    int sock;

    struct sockaddr_in server;
    sock = socket(AF_INET, SOCK_STREAM, 0);
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

void worker(int client_fd)
{
    char msg[] = "Hello!";
    if(send(client_fd, msg,  sizeof(msg), 0) == -1) {
        failure_counter++;
        perror("Unable to accept message");
    }
    
    char ans[256];
    if(recv(client_fd, ans, sizeof(ans), 0) == -1) {
        failure_counter++;
        perror("Unable to receive message");
    }
    printf("Received %s form client %d\n", ans, client_fd);

    if(shutdown(client_fd, SHUT_RDWR))
        perror("shutdown");
    if(close(client_fd))
        perror("close");
}

int main()
{
    printf("%d\n", getpid());
    int addr = inet_addr("127.0.0.1");
    int sock = create_server_sock(addr, 7777);

    if(listen(sock, 100))
    {
        perror("Unable to listen for connections");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    int client_fd;
    unsigned int size = sizeof(client);

    for(int i = 0; i < 100; i++) {
        client_fd = accept(sock, (struct sockaddr *)&client, &size);
        if(client_fd == -1) {
            perror("Unable to connect to client");
            continue;
        }

        int handler = fork();

        if(handler == 0) {
            worker(client_fd);
            return 0;
        }

        printf("%d requests handled\n", i + 1);

    }
    int stat = 0;
    while(wait(&stat) > 0) {}

    printf("%d failures in total", failure_counter);

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return EXIT_SUCCESS;
}