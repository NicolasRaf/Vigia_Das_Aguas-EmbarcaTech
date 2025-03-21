#include "wifi_manager.h"

char formatedIP[32];

// Inicializa o Wi-Fi
int initWifi() {
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    cyw43_arch_enable_sta_mode();
    return 0;
}

// Conecta ao Wi-Fi
int connectWifi() {
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect.\n");
        return -1;
    }
    printf("Connected.\n");
    printIpAddress();

    return 0;
}

// Imprime o endereço IP
void printIpAddress() {
    // Obtém o endereço IP do estado da interface de rede
    uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);

    // Formata o endereço IP em forma de string
    snprintf(formatedIP, sizeof(formatedIP), "%d.%d.%d.%d",
             ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

    // Imprime o endereço IP no console
    printf(formatedIP);
}
