#ifndef LEDS_BUTTONS_H
#define LEDS_BUTTONS_H

#include "pico/stdlib.h"
#include "setup.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

#define LED_RED_PIN 13
#define LED_GREEN_PIN 11

void initGpioButtonLeds();
void distanceFeedback(bool reading, float distance);
void buttons_callback(uint gpio, uint32_t events);

#endif
