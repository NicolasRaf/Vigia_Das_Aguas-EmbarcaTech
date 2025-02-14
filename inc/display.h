#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/i2c.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C // Endereço I2C do display
#define I2C_SDA 14          // Pino SDA
#define I2C_SCL 15          // Pino SCL
#define MAX_LINES 6
#define MAX_SCREENS 3
typedef struct {
    const char *text;
    uint32_t x;
    uint32_t y;
    uint32_t scale;
} DisplayLine;

typedef struct {
    DisplayLine lines[MAX_LINES];
} DisplayScreen;

typedef struct {
    const char *name;
    DisplayScreen screen;
} DisplayScreenDict;

extern ssd1306_t display;
extern DisplayScreenDict screens[MAX_SCREENS];

int initializeDisplay();
void showScreen(DisplayScreen screen);
void clearScreen();

#endif // DISPLAY_H