#include "leds_buttons.h"

void initGpioButtonLeds() {

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

void distanceFeedback(bool reading, float distance) {
    if (!reading) {
        gpio_put(LED_RED_PIN, 0);
        gpio_put(LED_GREEN_PIN, 0);
        return;
    }

    if (distance >= 0 && distance < 20) {
        gpio_put(LED_RED_PIN, 1);
        gpio_put(LED_GREEN_PIN, 0);

        playSoundStrong();
    } else if (distance >= 20 && distance < 40) {
        gpio_put(LED_RED_PIN, 1);
        gpio_put(LED_GREEN_PIN, 1);

        playSoundMedium();
    } else if (distance >= 40) {
        gpio_put(LED_RED_PIN, 0);
        gpio_put(LED_GREEN_PIN, 1);
    }
}

void buttons_callback(uint gpio, uint32_t events) {
    if (strcmp(currentScreenName, "Welcome Screen") == 0) {
        return;
    }

    if (gpio == BUTTON_A_PIN) {
        reading = !reading;

    } else if (gpio == BUTTON_B_PIN) {
        // Trocar entre as telas
        currentScreenIndex = (currentScreenIndex + 1) % MAX_SCREENS;
        (currentScreenIndex == 0) ? currentScreenIndex = 1 : currentScreenIndex;

        currentScreenName = screens[currentScreenIndex].name;
    }
}