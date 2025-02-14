#ifndef ULTRASONIC_H
#define ULTRASONIC_H


#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "led_button.h"

#define TRIG_PIN 17
#define ECHO_PIN 16
#define BUTTON_PIN 5

float mensureDistance();
void ledFeedback(float distance);

#endif // ULTRASONIC_H
