#ifndef SETUP_H
#define SETUP_H

#include <stdio.h> 
#include "string.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "wifi_manager.h"
#include "display.h"
#include "ultrasonic.h"
#include "leds_buttons.h"
#include "sound.h"

#define TRIG_PIN 17
#define ECHO_PIN 16
extern char *currentScreenName;
extern int currentScreenIndex;

void initializeSystem();
void buttons_callback(uint gpio, uint32_t events);
void initInterrupts();

#endif // SETUP_H
