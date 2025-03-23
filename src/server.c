#include "server.h"

struct tcp_pcb *pcb = NULL;
bool haveConnection = false;
int retries = 0;
#define MAX_RETRIES 5

// Callback para resposta do servidor
err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        printf("Conexão fechada pelo servidor.\n");
        tcp_close(tpcb);
        haveConnection = false;
        pcb = NULL;
        return ERR_OK;
    } else {
        printf("Resposta recebida: %s\n", (char*)p->payload);
        tcp_recved(tpcb, p->tot_len);
        pbuf_free(p);
        return ERR_OK;
    }
}

// Callback para erro na conexão TCP
void tcp_client_error(void *arg, err_t err) {
    printf("Erro na conexão TCP (%d). Tentando reconectar...\n", err);
    pcb = NULL;
    haveConnection = false;

    if (retries < MAX_RETRIES) {
        create_tcp_connection();
    } else {
        printf("Máximo de tentativas atingido. Abortando.\n");
    }
}

// Criar conexão TCP (usando o proxy)
void create_tcp_connection() {
    if (pcb != NULL) return;  // Já conectado
    retries++;

    // Substitua pelo seu proxy gerado pelo Railway
    const char *proxy_host = PROXY_HOST;  // Proxy gerado pelo Railway
    int proxy_port = PROXY_PORT;  // Porta do proxy

    ip_addr_t proxy_ip;

    // Resolução DNS para obter o IP do proxy
    if (dns_gethostbyname(proxy_host, &proxy_ip, NULL, NULL) != ERR_OK) {
        printf("Erro ao resolver DNS do proxy\n");
        return;
    }

    pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    tcp_err(pcb, tcp_client_error);  // Registrar callback de erro

    // Tente se conectar ao proxy
    err_t connect_err = tcp_connect(pcb, &proxy_ip, proxy_port, NULL);
    if (connect_err == ERR_OK) {
        printf("Conectado ao proxy!\n");
        haveConnection = true;
        tcp_recv(pcb, tcp_client_recv);
    } else {
        printf("Falha na conexão (%d). Tentativa %d/%d\n", connect_err, retries, MAX_RETRIES);
        tcp_abort(pcb);
        pcb = NULL;
    }
}

// Enviar dados ao servidor (usando o proxy)
void send_data_to_server(float waterLevel) {
    while (pcb == NULL || !haveConnection) {
        if (retries >= MAX_RETRIES) {
            printf("Erro: Não foi possível restabelecer a conexão após %d tentativas.\n", MAX_RETRIES);
            return;  // Aborta se o número máximo de tentativas de reconexão for atingido
        }

        printf("Conexão perdida. Tentando reconectar... (%d/%d)\n", retries, MAX_RETRIES);
        create_tcp_connection();  // Tenta reconectar
        sleep_ms(1000);
    }

    // Enviar uma requisição ao servidor, agora através do proxy
    retries = 0;
    char request[256];
    snprintf(request, sizeof(request),
        "GET /update?waterLevel=%.2f HTTP/1.1\r\nHost: vigia_das_aguas-server.railway.internal\r\nConnection: keep-alive\r\n\r\n",
        waterLevel);

    printf("Request enviada para o servidor!\n");

    if (tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY) != ERR_OK) {
        printf("Erro ao enviar dados. Fechando conexão...\n");
        tcp_abort(pcb);
        pcb = NULL;
        haveConnection = false;
        return;
    }

    if (tcp_output(pcb) != ERR_OK) {
        printf("Erro ao enviar dados (tcp_output). Fechando conexão...\n");
        tcp_abort(pcb);
        pcb = NULL;
        haveConnection = false;
        return;
    }
}

// Fechar conexão TCP
void close_tcp_connection() {
    if (pcb != NULL) {
        tcp_close(pcb);
        pcb = NULL;
        haveConnection = false;
        printf("Conexão TCP fechada.\n");
    }
}
