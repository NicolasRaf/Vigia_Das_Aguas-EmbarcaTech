#ifndef SERVER_H
#define SERVER_H

#include "lwip/tcp.h"
#include <string.h>

extern bool server_connected;

err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void send_data_to_server();

#endif // SERVER_H