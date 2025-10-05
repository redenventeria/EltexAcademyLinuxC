#include<netinet/in.h>

#define SERVER_PORT 8888
#define CLIENT_PORT 8889
#define ADDR INADDR_LOOPBACK

int udp_checksum(void *m, size_t n);

typedef struct {
    struct udphdr udp;
    char data[128];
} echo_request;

typedef struct {
    char _pad[20];
    //echo_response request;
    char buf[sizeof(echo_request)];
} echo_request_ip;

typedef struct {
    struct udphdr udp;
    size_t ord;
    char data[128];
} echo_response;

typedef struct {
    char _pad[20];
    //echo_response response;
    char buf[sizeof(echo_request)];
} echo_response_ip;

typedef struct {
    uint32_t ip;
    uint16_t port;
} client_id;
