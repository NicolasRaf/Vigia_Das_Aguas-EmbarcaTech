#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"

#define WIFI_SSID "Amora"
#define WIFI_PASSWORD "Naruto2021"


extern char formatedIP[]; // Buffer para armazenar o IP formatado como string

// Inicializa o Wi-Fi
int initWifi();

// Conecta ao Wi-Fi
int connectWifi();

// Imprime o endereço IP
void printIpAddress();

#endif // WIFI_MANAGER_H
