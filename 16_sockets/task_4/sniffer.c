#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#include<sys/socket.h>
#include<netinet/ip.h>

#define MAX_DGRAM 65536
#define DGRAM_DATA_OFFSET 20
#define DGRAM_LEN_OFFSET 4

int main()
{
    struct sockaddr_in sniffer;

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    while(1) {
        char msg[MAX_DGRAM];
        recvfrom(sock, msg, 256, 0, NULL, NULL);

        uint16_t length;
        length = (uint16_t)msg[DGRAM_LEN_OFFSET];
        length = ntohs(length);

        char beg[64 + 1];
        strncpy(beg, msg + DGRAM_DATA_OFFSET, 64);
        beg[64] = '\0';

        printf("Received %u bytes\n", (unsigned int)length);

        printf("First 64 bytes of data: ");
        for(size_t i = 0; i < 64; i++) {
            printf("%c", msg[i + DGRAM_DATA_OFFSET]);
        }
        printf("\n");
    }



    return 0;
}