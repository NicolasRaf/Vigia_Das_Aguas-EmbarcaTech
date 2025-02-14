#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "init.h"
#include "ultrasonic.h"

#define NUM_READINGS 10  // Quantidade de leituras a armazenar

// Variáveis globais
int reading = 1;                // 1: sensor ligado; 0: sensor desligado
int timeUntilNextRead = 5;      // Tempo para iniciar a próxima medição (em segundos)

// Buffer para armazenar os últimos valores lidos
float lastReadings[NUM_READINGS] = {0};
int readingCount = 0;   // Quantidade de leituras armazenadas (máximo NUM_READINGS)
int readingIndex = 0;   // Índice para a próxima inserção no buffer

void updateScreen() {
    char distanceText[30];
    char otherText[20];
    char timeText[30];

    // Exemplo de outro valor exibido na tela
    sprintf(otherText, "Other Value: %.2f", 1.0);

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
                    sprintf(distanceText, "Dist. Media: %.2f cm", averageDistance);
                    sprintf(timeText, "Prox leitura: %d seg", timeUntilNextRead);
                    screens[i].screen.lines[2].text = distanceText;
                    screens[i].screen.lines[3].text = timeText;
                    
                    // Linha 4 indica se a medição está em andamento
                    screens[i].screen.lines[4].text = (measuring ? "Em andamento..." : "");
                }
            } else if (strcmp(currentScreenName, "Screen3") == 0) {
                // Exemplo para outra tela
                sprintf(distanceText, "Valor: %.2f", averageDistance);
                screens[i].screen.lines[0].text = distanceText;
            }
    
            // Feedback dos LEDs e exibição da tela (comum a todas as telas)
            ledFeedback(reading, averageDistance);
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

        // Atualiza o contador para a próxima leitura, se não estiver medindo
        if (!measuring && reading) {
            timeUntilNextRead--;
            if (timeUntilNextRead < 0) {
                timeUntilNextRead = 5;  // Reset do contador para 5 segundos
                // Quando o tempo expira, seta a flag para iniciar a medição
                startMeasurement = true;
            }
        }

        // Se a flag de medição estiver setada e não estiver medindo, inicia a medição
        if (startMeasurement && !measuring) {
            measuring = true;
            printf("Iniciando a medição de distância...\n");

            // Verifica se o sensor está ligado (reading == 1)
            if (reading == 1) {
                float newReading = measureDistance();
                if (newReading != -1) {
                    // Armazena o novo valor no buffer circular
                    lastReadings[readingIndex] = newReading;
                    readingIndex = (readingIndex + 1) % NUM_READINGS;
                    if (readingCount < NUM_READINGS) {
                        readingCount++;
                    }
                    // Calcula a média dos valores armazenados
                    float sum = 0;
                    for (int i = 0; i < readingCount; i++) {
                        sum += lastReadings[i];
                    }
                    averageDistance = sum / readingCount;
                    printf("Nova medição: %.2f cm, média: %.2f cm\n", newReading, averageDistance);
                } else {
                    printf("Erro na medição!\n");
                }
            } else {
                printf("Sensor desligado, medição não efetuada.\n");
            }
            // Reseta as flags para a próxima medição
            measuring = false;
            startMeasurement = false;
        } 

        sleep_ms(1000); // Espera 1 segundo antes de atualizar a tela novamente
    }

    return 1;
}
