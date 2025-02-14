#ifndef INIT_H
#define INIT_H

#include <stdio.h> 
#include "wifi_manager.h"
#include "init.h"
#include "server.h"
#include "display.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "display.h"
#include "ultrasonic.h"
#include "led_button.h"

#define TRIG_PIN 17
#define ECHO_PIN 16
extern int reading;

void initializeSystem();
void initHttpServer();

void initGpio();
void initI2C();
void clearScreen();
void initHcsr04();
void initInterrupts();

#endif // INIT_H
