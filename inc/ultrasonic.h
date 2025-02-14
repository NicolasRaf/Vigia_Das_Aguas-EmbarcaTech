#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "leds_buttons.h"

#define TRIG_PIN 17
#define ECHO_PIN 16
#define NUM_READINGS 10  // Quantidade de leituras a armazenar

// Variáveis globais
extern float averageDistance;
extern bool measuring;
extern volatile bool startMeasurement;
extern int reading;                // 1: sensor ligado; 0: sensor desligado
extern int timeUntilNextRead;      // Tempo para iniciar a próxima medição (em segundos)
extern float lastReadings[]; // Buffer para armazenar os últimos valores lidos
extern int readingCount;   // Quantidade de leituras armazenadas (máximo NUM_READINGS)
extern int readingIndex;   // Índice para a próxima inserção no buffer

void initHcsr04();
float measureDistance();
void measurementControler();
void readingCounter();

#endif // ULTRASONIC_H
