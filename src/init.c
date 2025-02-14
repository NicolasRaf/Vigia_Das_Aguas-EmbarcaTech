#include "init.h"

// Inicializa o servidor HTTP
void initHttpServer() {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, http_server_accept);
}

// Inicializa os pinos GPIO
void initGpio() {

    // LED Vermelho
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    // LED Verde
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    // Botão A
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  // Habilita o pull-up para evitar falsos acionamentos
}

// Inicializa a interface I2C
void initI2C() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

// Inicializa o sistema
void initializeSystem() {
    stdio_init_all();
    initGpio();
    initI2C();
    initializeDisplay();
    //initWifiConnection();
    initHcsr04();
    initInterrupts();

    clearScreen();
}

void initHcsr04() {
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

// Função de callback para interrupção do botão

void switchReading() {
    reading = !reading;
}

void initInterrupts() {

    // Inicializa a interrupção do botão para limpar a tela
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, clearScreen);

    // Troca o estado de leitura quando o botão for pressionado
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, switchReading);
}
