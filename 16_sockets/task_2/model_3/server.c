#include<stdio.h>
#include<string.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>

#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/syscall.h>
#include<pthread.h>

#define CLIENT_N 100
#define WORKER_COUNT 5
#define PORT 7778

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
    int *mq = (int *)arg;
    while(1) {

        char stringified[10];
        int err = mq_receive(*mq, stringified, 10, NULL);
        if(err == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
        client_fd = atoi(stringified);
        if(client_fd == -1)
            break;
        response(client_fd);
    }

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

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4;

    mqd_t queue = mq_open(
        "/a",
        O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR,
        &attr
    );
    if(queue == -1)
        perror("queue");
    
    pthread_t threads[WORKER_COUNT];
    for(int i = 0; i < WORKER_COUNT; i++) {
        pthread_create(&threads[i], NULL, worker, &queue);
    }

    for(int i = 0; i < CLIENT_N; i++) {

        int client_fd = accept(sock, (struct sockaddr *)&client, &size);
        
        if(client_fd == -1) {
            perror("Unable to connect to client");
            continue;
        }

        int a;
        char stringified[10];
        sprintf(stringified, "%d", client_fd);
        a = mq_send(queue, stringified, sizeof(stringified), 1);
        if(a == -1) {
            perror("mq_send");
        }
        printf("%d <%s> \n", a, stringified);
        printf("%d connections accepted so far\n", i);


    }

    char end[] = "-1";
    int a;
    for(int i = 0; i < WORKER_COUNT; i++) {
        a = mq_send(queue, end, sizeof(end), 1);
    }
    if(a == -1) {
        perror("mq_send");
    }
    for(int i = 0; i < WORKER_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    if(shutdown(sock, SHUT_RDWR))
        perror("shutdown");
    if(close(sock))
        perror("close");

    printf("%d failures in total\n", failure_counter);

    return EXIT_SUCCESS;
}