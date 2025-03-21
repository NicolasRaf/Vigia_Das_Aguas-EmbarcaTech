#include "server.h"

bool server_connected = false;

// Função de callback para lidar com a resposta do servidor
err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    } else {
        tcp_recved(tpcb, p->tot_len);
        pbuf_free(p);
        return ERR_OK;
    }
}

// Função para enviar os dados de temperatura para o servidor
void send_data_to_server() {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    ip_addr_t server_ip;
    IP4_ADDR(&server_ip, 10, 30, 44, 103); // IP do servidor

    if (tcp_connect(pcb, &server_ip, 5000, NULL) != ERR_OK) { // Porta 5000
        printf("Erro ao conectar ao servidor\n");
        tcp_abort(pcb);
        return;
    }

    if (!server_connected) {
        printf("Conectado ao Servidor!\n");
        server_connected = true;
    }

    char request[256];
    snprintf(request, sizeof(request), 
        "GET /update?temperature=%.2f HTTP/1.1\r\nHost: 10.30.44.103\r\n\r\n", 
        "temperature");
    printf("Request: %s\n", request);

    if (tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY) != ERR_OK) {
        printf("Erro ao enviar dados\n");
        tcp_abort(pcb);
        return;
    }

    if (tcp_output(pcb) != ERR_OK) {
        printf("Erro ao enviar dados (tcp_output)\n");
        tcp_abort(pcb);
        return;
    }

    tcp_recv(pcb, tcp_client_recv);
}