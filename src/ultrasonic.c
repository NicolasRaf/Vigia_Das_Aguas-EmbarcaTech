#include "ultrasonic.h"

// Função para medir a distância com o sensor ultrasonico
float mensureDistance() {
    // Enviar pulso de 10µs para o TRIG
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Medir tempo do pulso no ECHO
    while (gpio_get(ECHO_PIN) == 0); // Aguarda HIGH
    absolute_time_t start = get_absolute_time();

    while (gpio_get(ECHO_PIN) == 1); // Aguarda LOW
    absolute_time_t end = get_absolute_time();

    // Calcular tempo decorrido em microssegundos
    uint64_t duration = absolute_time_diff_us(start, end);

    // Converter tempo em distância (velocidade do som = 343 m/s)
    float distance_cm = (duration * 0.0343) / 2;
    
    return distance_cm;
}

void ledFeedback(float distance) {
    if (distance >= 0 && distance < 20) {
        // Progressivamente acende o verde
        gpio_put(LED_RED_PIN, 1);
        gpio_put(LED_GREEN_PIN, 0);
    } else if (distance >= 20 && distance < 40) {
        // Acende o verde e o vermelho ao mesmo tempo (amarelo)
        gpio_put(LED_GREEN_PIN, 1);
        gpio_put(LED_RED_PIN, 1);
    } else if (distance >= 40 ) {
        // Acende apenas o verde
        gpio_put(LED_RED_PIN, 0);
        gpio_put(LED_GREEN_PIN, 1);
    } 
}

