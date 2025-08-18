#include<stdio.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>

#define CLIENT_N 5
#define B_PORT 9999

void client(int cli_adress)
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

    int addr = INADDR_ANY;
    struct ip_mreqn mcast_addr;
    mcast_addr.imr_address.s_addr = cli_adress;
    mcast_addr.imr_multiaddr.s_addr = addr;
    mcast_addr.imr_ifindex = 1;
    if(setsockopt(
        sock,
        SOL_SOCKET,
        IP_ADD_MEMBERSHIP,
        &mcast_addr,
        sizeof(mcast_addr)
    )) perror("sotsockopt");
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = addr;
    server.sin_port = 7777;
    unsigned int serv_size = sizeof(server);

    struct sockaddr_in multicast;
    multicast.sin_family = AF_INET;
    multicast.sin_addr.s_addr = addr;
    multicast.sin_port = 9999;
    if(bind(
        sock,
        (struct sockaddr *)&multicast,
        sizeof(multicast)
    )) perror("bind");    
    
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
            client(inet_addr("192.168.0.0") + htonl(i));
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