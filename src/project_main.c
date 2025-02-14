#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "setup.h"
#include "ultrasonic.h"

void updateScreen() {
    char distanceText[30];
    char timeText[30];

    for (int i = 0; i < MAX_SCREENS; i++) {
        if (strcmp(screens[i].name, currentScreenName) == 0) {
            if (strcmp(currentScreenName, "Main Screen") == 0) {
                // Se o sensor estiver desligado, exibe mensagem apropriada
                if (reading == 0) {
                    sprintf(distanceText, "Leitura do sensor");
                    sprintf(timeText, "desligada!");
                    screens[i].screen.lines[2].text = distanceText;
                    screens[i].screen.lines[3].text = timeText;
                } else {
                    // Sensor ligado: mostra a média e o tempo restante para a próxima leitura
                    if (mode == 0){
                        sprintf(distanceText, "Dist. Media: %.2f cm", averageDistance);
                        distanceFeedback(reading, averageDistance);
                    } else {
                        int lastValueIndex = (readingIndex - 1 + NUM_READINGS) % NUM_READINGS;

                        sprintf(distanceText, "Dist. Atual: %.2f m", lastReadings[lastValueIndex]);
                        distanceFeedback(reading, lastReadings[lastValueIndex]);

                    }
                    sprintf(timeText, "Prox leitura: %d seg", timeUntilNextRead);
                    screens[i].screen.lines[2].text = distanceText;
                    screens[i].screen.lines[3].text = timeText;
                }
            } else if (strcmp(currentScreenName, "Logs") == 0) {
                static char logBuffer[5][23];  // Buffer com 23 caracteres para incluir '\0'

                // Preencher as 5 linhas de log
                for (int j = 0; j < 5; j++) {
                    // Calcular os índices corretamente com base no número de leituras realizadas
                    int leftIdx = (readingIndex - 1 - j + NUM_READINGS) % NUM_READINGS;  // Índice para a esquerda
                    int rightIdx = (readingIndex - 6 - j + NUM_READINGS) % NUM_READINGS; // Índice para a direita

                    // Exibe o valor mais recente à esquerda e o valor mais antigo à direita
                    snprintf(logBuffer[j], 23, "%2d - %.1f |%2d - %.1f", 
                            (j + 1), lastReadings[(readingIndex - 1 - j + NUM_READINGS) % NUM_READINGS],
                            (j + 6), lastReadings[(readingIndex - 6 - j + NUM_READINGS) % NUM_READINGS]);

                    screens[i].screen.lines[j + 1].text = logBuffer[j];  // Coloca o texto na tela
                }
            }
    
            // Feedback dos LEDs e exibição da tela (comum a todas as telas)
            showScreen(screens[i].screen);
            break;
        }
    }
}


int main() {
    initializeSystem();
    currentScreenName = "Welcome Screen";
    showScreen(screens[0].screen);

    sleep_ms(3000);
    currentScreenName = "Main Screen";

    while (true) {
        updateScreen();
    
        readingCounter(); // Atualiza o contador para a próxima leitura, se não estiver medindo
        measurementControler(); // Controle da medição

        sleep_ms(1000); // Espera 1 segundo antes de atualizar a tela novamente
    }

    return 1;
}
