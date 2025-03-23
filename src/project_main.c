#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "setup.h"
#include "ultrasonic.h"

void updateScreen() {
    char distanceText[30];
    char timeText[30];
    char slopeText[30];
    char wifiText[30];
    char serverConnectionText[30];

    for (int i = 0; i < MAX_SCREENS; i++) {
        if (strcmp(screens[i].name, currentScreenName) == 0) {
            if (strcmp(currentScreenName, "Main Screen") == 0) {
                // Se o sensor estiver desligado, exibe mensagem apropriada
                if (reading == 0) {
                    sprintf(distanceText, "Leitura do sensor");
                    sprintf(timeText, "desligada!");
                    screens[i].screen.lines[1].text = distanceText;
                    screens[i].screen.lines[2].text = timeText;

                    for (int j = 3; j < MAX_LINES; j++) {
                        screens[i].screen.lines[j].text = "";
                    }

                    distanceFeedback(reading, averageDistance);
                } else {

                    // Sensor ligado: mostra a média e o tempo restante para a próxima leitura
                    if (readingMode == 0){
                        sprintf(distanceText, "Dist. Media: %.2f cm", averageDistance);
                        distanceFeedback(reading, averageDistance);

                        screens[i].screen.lines[5].text = "";
                    } else if (readingMode == 1) {
                        int lastValueIndex = (readingIndex - 1 + NUM_READINGS) % NUM_READINGS;

                        sprintf(distanceText, "Dist. Atual: %.2f cm", lastReadings[lastValueIndex]);
                        distanceFeedback(reading, lastReadings[lastValueIndex]);

                        screens[i].screen.lines[5].text = "- Sujeito a Ruido";
                    }

                    if (timeUntilNextRead > 60) {
                        sprintf(timeText, "Prox leitura: %d min", timeUntilNextRead / 60);
                    } else {
                     sprintf(timeText, "Prox leitura: %d seg", timeUntilNextRead);
                    }
                    sprintf(slopeText, "Tx de Aum.: %.1f cm/s", slope);

                    screens[i].screen.lines[1].text = "";
                    screens[i].screen.lines[2].text = distanceText;
                    screens[i].screen.lines[3].text = timeText;
                    screens[i].screen.lines[4].text = slopeText;
                }
            } else if (strcmp(currentScreenName, "Logs") == 0) {
                (readingMode) ? distanceFeedback(reading, averageDistance) : 
                distanceFeedback(reading, lastReadings[(readingIndex - 1 + NUM_READINGS) % NUM_READINGS]);

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
            }  else if (strcmp(currentScreenName, "Options") == 0) {
                sprintf(distanceText, "Modo de operacao: %d", readingMode);
                screens[i].screen.lines[1].text = distanceText;
                char time[30];

                if (readingTimes[currenteTimeOption] > 60) {
                    sprintf(time, "%d min", readingTimes[currenteTimeOption] / 60);
                } else {
                    sprintf(time, "%d seg", readingTimes[currenteTimeOption]);
                }

                sprintf(timeText, "Delay: %s ", time);
                sprintf(wifiText, "IP: %s", formatedIP);
                sprintf(serverConnectionText, "Server: %s", haveConnection ? "Conectado" : "Desconectado");
            

                screens[i].screen.lines[2].text = timeText;
                screens[i].screen.lines[3].text = wifiText;
                screens[i].screen.lines[4].text = serverConnectionText;
            }
    
            // Feedback dos LEDs e exibição da tela (comum a todas as telas)
            showScreen(screens[i].screen);
            break;
        }
    }
}


int main() {
    initializeSystem();

    showScreen(screens[0].screen);
    sleep_ms(3000);
    connectWifi();
    create_tcp_connection();

    currentScreenName = "Main Screen";
    while (true) {
        updateScreen();
    
        readingCounter(); // Atualiza o contador para a próxima leitura, se não estiver medindo
        measurementControler(); // Controle da medição

        sleep_ms(1000); // Espera 1 segundo antes de atualizar a tela novamente
    }

    return 1;
}
