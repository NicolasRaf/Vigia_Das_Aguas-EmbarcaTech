#ifndef INIT_H
#define INIT_H

#include <stdio.h> 
#include "wifi_manager.h"
#include "server.h"
#include "display.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ultrasonic.h"
#include "led_button.h"

#define TRIG_PIN 17
#define ECHO_PIN 16
extern int reading;
extern char *currentScreenName;

void initializeSystem();
void initHttpServer();

void initHttpServer();
void initGpio();
void initI2C();
void initializeSystem();
void initHcsr04();
void switchReading();
void buttons_callback(uint gpio, uint32_t events);
void initInterrupts();

#endif // INIT_H
