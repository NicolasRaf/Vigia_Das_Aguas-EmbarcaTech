#ifndef SERVER_H
#define SERVER_H

#include "lwip/tcp.h"
#include "lwip/dns.h"
#include <string.h>
#include "pico/stdlib.h"

#define MAX_RETRIES 5 

#ifndef PROXY_PORT
    #define PROXY_PORT 1111
#endif

#ifndef PROXY_HOST
    #define PROXY_HOST ""
#endif

extern bool haveConnection;
extern int retries;

err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void send_data_to_server(float value);
void create_tcp_connection();
void close_tcp_connection();

#endif // SERVER_H