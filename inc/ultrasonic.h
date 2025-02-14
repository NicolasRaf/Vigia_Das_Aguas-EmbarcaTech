#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "led_button.h"

#define TRIG_PIN 17
#define ECHO_PIN 16

extern float averageDistance;
extern bool measuring;
extern volatile bool startMeasurement;

float measureDistance();
void ledFeedback(bool reading, float distance);

#endif // ULTRASONIC_H
