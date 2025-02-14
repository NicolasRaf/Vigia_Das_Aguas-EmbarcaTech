#include "server.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "display.h"


const char *html_page = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n\r\n"
                        "<!DOCTYPE html><html><head>"
                        "<style>"
                        "body { font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 0; text-align: center; }"
                        "h1 { color: #333; margin-top: 50px; }"
                        "button { background-color: #4557a0; /* Verde */ color: white; border: none; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 5px; transition: background-color 0.3s ease; }"
                        "button:hover { background-color:#4557a0; }"
                        "p { font-size: 18px; color: #555; }"
                        "</style>"
                        "</head><body>"
                        "<h1>Pico W LED & Music Control</h1>"
                        "<p><a href=\"/?led=on\"><button>Ligar LED</button></a></p>"
                        "<p><a href=\"/?led=off\"><button>Desligar LED</button></a></p>"
                        "<p><a href=\"/?music=play\"><button>Play Music</button></a></p>"
                        "</body></html>";


err_t http_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    char *data = (char*)p->payload;

    if (strstr(data, "GET /?led=on")) {
        gpio_put(13, 1);
    } else if (strstr(data, "GET /?led=off")) {
        gpio_put(13, 0);
    } else if (strstr(data, "GET /?music=play")) {
        playMusic();
    }

    tcp_write(tpcb, html_page, strlen(html_page), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    pbuf_free(p);
    return ERR_OK;
}

err_t http_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_server_recv);
    return ERR_OK;
}
