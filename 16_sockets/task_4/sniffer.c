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
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    while(1) {
        char msg[MAX_DGRAM];
        struct  sockaddr_in addr;
        int len = sizeof(addr);
        
        int recv_len = recvfrom(
            sock,
            msg,
            256,
            0,
            (struct sockaddr *)&addr,
            &len
        );

        char beg[64 + 1];
        memcpy(beg, msg + DGRAM_DATA_OFFSET, 64);
        beg[64] = '\0';

        printf("Received %u bytes\n", (unsigned int)recv_len);

        int beg_len = 64;
        if(recv_len < 64) {
            beg_len = recv_len - 20;
        }
        printf("First %d bytes of data: ", beg_len);
        for(size_t i = 0; i < beg_len; i++) {
            printf("%c", beg[i]);
        }
        printf("\n");
    }



    return 0;
}