#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/syscall.h>
#include<pthread.h>

#define CLIENT_N 100
#define WORKER_COUNT 5
#define PORT 7777

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

void response(int client_fd)
{
    char msg[] = "Hello!";
    if(send(client_fd, msg, sizeof(msg), 0) == -1) {
        failure_counter++;
        perror("Unable to send message");
    }
    
    char ans[256];
    if(recv(client_fd, ans, sizeof(ans), 0) == -1) {
        failure_counter++;
        perror("Unable to receive message");
    }

    printf("Received %s form client with socket fd %d\n", ans, client_fd);

    if(shutdown(client_fd, SHUT_RDWR))
        perror("shutdown");
    if(close(client_fd))
        perror("close of sock");
}

void *worker(void *arg) {

    int client_fd;
    int *pip = (int *)arg;
    while(1) {

        int err = read(*pip, &client_fd, sizeof(client_fd));
        if(err == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        else if (err == 0) {
            printf("Worker received EOF, exiting...\n");
            break;
        }
        response(client_fd);
    }
    //close(*pip);

    return NULL;
}

int main()
{
    printf("%d\n", getpid());
    int addr = inet_addr("127.0.0.1");
    int sock = create_server_sock(addr, PORT);

    if(listen(sock, 100))
    {
        perror("Unable to listen for connections");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    unsigned int size = sizeof(client);

    int pipes[WORKER_COUNT][2];
    pthread_t threads[WORKER_COUNT];
    for(int i = 0; i < WORKER_COUNT; i++) {
        if(pipe(pipes[i])) {
            perror("pipe");
        }
        
        pthread_create(&threads[i], NULL, worker, &pipes[i][0]);

    }

    for(int i = 0; i < CLIENT_N; i++) {

        int client_fd = accept(sock, (struct sockaddr *)&client, &size);
        
        if(client_fd == -1) {
            perror("Unable to connect to client");
            continue;
        }

        if(write(pipes[i % WORKER_COUNT][1], &client_fd, sizeof(client_fd)) != sizeof(client_fd)) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        printf("%d connections accepted so far\n", i);


    }

    for(int i = 0; i < WORKER_COUNT; i++) {
        close(pipes[i][1]);
        pthread_join(threads[i], NULL);
        close(pipes[i][0]);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    printf("%d failures in total", failure_counter);

    return EXIT_SUCCESS;
}