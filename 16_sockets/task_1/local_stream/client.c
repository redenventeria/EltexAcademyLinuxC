#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int sock;
    struct sockaddr_un client;

    sock = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }
    
    client.sun_family = AF_LOCAL;
    strncpy(client.sun_path, "socket", sizeof (client.sun_path));
    struct sockaddr *client_saddr = (struct sockaddr *)&client;
    connect(sock, client_saddr, sizeof(client_saddr));
    
    
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