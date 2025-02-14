#include "init.h"

char *currentScreenName;

void initHttpServer() {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, http_server_accept);
}

void initGpio() {
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

void initI2C() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void initializeSystem() {
    stdio_init_all();
    initGpio();
    initI2C();
    initializeDisplay();
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

void switchReading() {
    reading = !reading;
}

void buttons_callback(uint gpio, uint32_t events) {
    if (strcmp(currentScreenName, "Welcome Screen") == 0) {
        return;
    }

    if (gpio == BUTTON_A_PIN) {
        switchReading();
    } else if (gpio == BUTTON_B_PIN) {
        clearScreen();
        ssd1306_show(&display);

        if (strcmp(currentScreenName, "Main Screen") == 0) {
            currentScreenName = "Screen3";
        } else {
            currentScreenName = "Main Screen";
        }
    }
}

void initInterrupts() {
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_RISE, true, buttons_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_RISE, true, buttons_callback);
}
