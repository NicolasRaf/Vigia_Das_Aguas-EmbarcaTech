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

int initializeDisplay();
void showText(const char *texto, uint32_t x, uint32_t y, uint32_t scale, bool clear);
void clearScreen();
void clearLine(uint32_t y);

extern ssd1306_t display;