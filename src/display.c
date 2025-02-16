#include "display.h"
ssd1306_t display;

void initI2CDisplay() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

DisplayScreenDict screens[MAX_SCREENS] = {
    {
        "Welcome Screen", {
            {
                {"==== Bem-vindo! =======", 0, 0, 1},
                {"", 0, 10, 1},
                {"Botao A: Interacao", 0, 20, 1},
                {"", 0, 25, 1},
                {"Botao B: Alterna Telas", 0, 40, 1},
                {"=======================", 0, 55, 1}
            }
        }
    },
    {
        "Main Screen", {
            {
                {"===== Principal ======", 0, 0, 1},
                {"", 0, 10, 1},
                {"", 0, 20, 1},
                {"", 0, 30, 1},
                {"", 0, 40, 1},
                {"=======================", 0, 55, 1}
            }
        }
    },
    {
        "Logs", {
            {
                {"== Ultimos 10 Logs ====", 0, 0, 1},
                {"", 0, 15, 1},
                {"", 0, 25, 1},
                {"", 0, 35, 1},
                {"", 0, 45, 1},
                {"", 0, 55, 1}
            }
        }
    },
    {
        "Options", {
            {
                {"======= Opcoes =======", 0, 0, 1},
                {"Modo de operacao:", 0, 15, 1},
                {"", 0, 25, 1},
                {"Delay:", 0, 35, 1},
                {"", 0, 45, 1},
                {"======================", 0, 55, 1}
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
