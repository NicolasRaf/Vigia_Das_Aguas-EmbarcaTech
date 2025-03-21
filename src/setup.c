#include "setup.h"

char *currentScreenName;
int currentScreenIndex = 0;

void initializeSystem() {
    stdio_init_all();

    initI2CDisplay();
    initializeDisplay();
    initWifi();
    initSound();
    initGpioButtonLeds();
    initHcsr04();
    initInterrupts();
    clearScreen();
}


void initInterrupts() {
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_RISE, true, buttons_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_RISE, true, buttons_callback);
}
