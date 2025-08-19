#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netinet/ip.h>

int main()
{
    struct sockaddr_in addr;

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;



    while(1) {
        char msg[] = "HELLO FROM SERVER!!!";
        sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&addr, sizeof(addr));
        sleep(1);
    }



    return 0;
}