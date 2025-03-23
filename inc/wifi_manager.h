#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"

#ifndef WIFI_SSID
    #define WIFI_SSID "SSID"
#endif

#ifndef WIFI_PASSWORD
    #define WIFI_PASSWORD "PASSWORD"
#endif

extern char formatedIP[]; // Buffer para armazenar o IP formatado como string

// Inicializa o Wi-Fi
int initWifi();

// Conecta ao Wi-Fi
int connectWifi();

// Imprime o endereço IP
void printIpAddress();

#endif // WIFI_MANAGER_H
