#include "display.h"

ssd1306_t display;
int initializeDisplay() {
    if (!ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, i2c1)) { 
        printf("Falha ao inicializar o display SSD1306\n");
        return 1;
    }
}

void showText(const char *texto, uint32_t x, uint32_t y, uint32_t scale, bool clear){
    if (clear) clearScreen();
    ssd1306_draw_string(&display, x, y, scale, texto);
    ssd1306_show(&display);
}

void clearScreen(){
    ssd1306_clear(&display);
}