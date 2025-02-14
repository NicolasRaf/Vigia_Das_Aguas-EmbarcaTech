#ifndef SERVER_H
#define SERVER_H

#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "music.h"

// Prototipos das funções
err_t http_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
err_t http_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);

#endif // SERVER_H
