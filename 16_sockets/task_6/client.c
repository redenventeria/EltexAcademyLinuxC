#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/udp.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdint.h>

#include"common.h"

char* create_packet(size_t *size) {
    struct ip hdr_ip;
    struct udphdr hdr_udp;
    char msg[] = "Hello from IPPROTO_RAW socket!!!";

    *size = sizeof(msg) + sizeof(hdr_udp) + sizeof(hdr_ip);

    hdr_udp.source = htons(CLIENT_PORT);
    hdr_udp.dest   = htons(SERVER_PORT);
    hdr_udp.check  = 0;
    hdr_udp.len    = htons(sizeof(msg) + 8);

    hdr_ip.ip_v   = 4;
    hdr_ip.ip_hl  = 5;
    hdr_ip.ip_tos = 0b00010000;
  //hdr_ip.ip_len = htons(*size); // Always filled in
    hdr_ip.ip_id  = 0; // Will be filled in
    hdr_ip.ip_off = 0;
    hdr_ip.ip_ttl = 10;
    hdr_ip.ip_p   = 17;
   //hdr_ip.ip_sum = 0 // Always filled in
    hdr_ip.ip_src.s_addr = 0; // Will be filled in
    hdr_ip.ip_dst.s_addr = htonl(ADDR);

    char *data = malloc(*size);

    size_t loc = 0;
    memcpy(data, &hdr_ip, sizeof(hdr_ip));
    loc += sizeof(hdr_ip);
    memcpy(data + loc, &hdr_udp, sizeof(hdr_udp));
    loc += sizeof(hdr_udp);
    memcpy(data + loc, msg, sizeof(msg));
    loc += sizeof(msg);
    if(loc != *size) {
        printf("Wrong sizes of packet components!\n");
    }

    return data;
}

void sendraw(int sock, struct sockaddr_in server) {

    size_t pack_size;
    char *packet = create_packet(&pack_size);

    if(sendto(
        sock,
        packet,
        pack_size,
        0,
        (struct sockaddr *)&server,
        sizeof(server)
    ) < 0) {
        perror("Message didnt sent!");
    }

    free(packet);
}

int main()
{

    int sock;
    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(sock == -1) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(ADDR);
    server.sin_port = 0;

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(ADDR);
    client.sin_port = 0;
    socklen_t cli_size = sizeof(client);

    if(bind(sock, (struct sockaddr*)&client, cli_size)) {
        perror("Unable to bind!");
    }
    
    sendraw(sock, server);

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return EXIT_SUCCESS;

}