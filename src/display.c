#include "display.h"


ssd1306_t display;

DisplayScreenDict screens[MAX_SCREENS] = {
    {
        "Welcome Screen", {
            {
                {"=======================", 0, 0, 1},
                {"", 0, 10, 1},
                {"", 0, 20, 1},
                {"Seja bem-vindo!", 0, 25, 1},
                {"", 0, 40, 1},
                {"=======================", 0, 55, 1}
            }
        }
    },
    {
        "Main Screen", {
            {
                {"=======================", 0, 0, 1},
                {"", 0, 10, 1},
                {"", 0, 20, 1},
                {"", 0, 30, 1},
                {"", 0, 40, 1},
                {"=======================", 0, 55, 1}
            }
        }
    },
    {
        "Screen3", {
            {
                {"Tela 3 - Linha 1", 0, 0, 1},
                {"Tela 3 - Linha 2", 0, 10, 1},
                {"Tela 3 - Linha 3", 0, 20, 1},
                {"Tela 3 - Linha 4", 0, 30, 1},
                {"Tela 3 - Linha 5", 0, 40, 1},
                {"Tela 3 - Linha 6", 0, 50, 1}
            }
        }
    }
};

int initializeDisplay() {
    if (!ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, i2c1)) { 
        printf("Falha ao inicializar o display SSD1306\n");
        return 1;
    }
    return 0;
}

void showScreen(DisplayScreen screen) {
    clearScreen();
    for (int i = 0; i < MAX_LINES; i++) {
        ssd1306_draw_string(&display, screen.lines[i].x, screen.lines[i].y, screen.lines[i].scale, screen.lines[i].text);
    }
    ssd1306_show(&display);
}

void clearScreen() {
    ssd1306_clear(&display);
}
