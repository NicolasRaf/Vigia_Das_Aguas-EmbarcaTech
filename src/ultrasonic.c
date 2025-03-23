#include "ultrasonic.h"

float averageDistance = 0;        
bool measuring = false;            
volatile bool startMeasurement = true;  
float lastReadings[NUM_READINGS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float slope = 0;
int reading = 1;             
int readingTimes[TIMES_DELAY] = {1, 30, 300, 600, 1800}; 
int currenteTimeOption = 0; 
int timeUntilNextRead = 1;     
int readingCount = 0;  
int readingIndex = 0;  

void initHcsr04() {
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);

    // Inicia o sensor desligado
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

float measureDistance() {
    // Enviar pulso de 10µs para o TRIG
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    const uint64_t timeout_us = 20000; // 20 ms
    absolute_time_t deadline = make_timeout_time_us(timeout_us);

    // Aguardar o pino ECHO ir para HIGH com timeout
    while (gpio_get(ECHO_PIN) == 0) {
        if (get_absolute_time() > deadline) {
            printf("Timeout esperando ECHO subir\n");
            return -1;
        }
    }
    absolute_time_t start = get_absolute_time();

    // Reconfigurar o deadline para aguardar o LOW
    deadline = make_timeout_time_us(timeout_us);
    while (gpio_get(ECHO_PIN) == 1) {
        if (get_absolute_time() > deadline) {
            printf("Timeout esperando ECHO descer\n");
            return -1;
        }
    }
    absolute_time_t end = get_absolute_time();

    // Calcular o tempo decorrido em microssegundos
    uint64_t duration = absolute_time_diff_us(start, end);

    // Converter o tempo em distância (velocidade do som = 343 m/s)
    float distance_cm = (duration * 0.0343) / 2;
    
    return distance_cm;
}

void measurementControler() {
    if (startMeasurement && !measuring) {
        measuring = true;
        printf("Iniciando a medição de distância...\n");

        // Verifica se o sensor está ligado (reading == 1)
        if (reading == 1) {
            float newReading = measureDistance();

            if (newReading > -1) {
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
                calcGrowthRateMultiPoints();

                if (haveConnection || retries < MAX_RETRIES) send_data_to_server(newReading);

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
}

void readingCounter() {
    if (!measuring && reading) {
        timeUntilNextRead--;
        if (timeUntilNextRead < 0) {
            timeUntilNextRead = readingTimes[currenteTimeOption];  // Reset do contador para 5 segundos
            // Quando o tempo expira, seta a flag para iniciar a medição
            startMeasurement = true;
        }
    }
}

// Calcula a taxa de crescimento (em cm/s) usando múltiplos pontos com regressão linear
float calcGrowthRateMultiPoints() {
    // Se não houver pelo menos 2 medições, não é possível calcular a taxa.
    if (readingCount < 2) {
        return 0.0;
    }

    int n = readingCount;                  // Número de medições válidas
    float dt = (float)readingTimes[currenteTimeOption];  // Intervalo de tempo entre medições (em segundos)
    
    // Reorganiza as medições na ordem cronológica (do mais antigo para o mais recente)
    float orderedReadings[NUM_READINGS];
    if (readingCount < NUM_READINGS) {
        // Se o buffer não estiver cheio, as medições já estão em ordem (índices 0..readingCount-1)
        for (int i = 0; i < n; i++) {
            orderedReadings[i] = lastReadings[i];
        }
    } else {
        // Se o buffer estiver cheio, o índice atual aponta para o próximo valor a ser escrito.
        // Portanto, o valor mais antigo está em readingIndex e os demais seguem de forma circular.
        for (int i = 0; i < n; i++) {
            orderedReadings[i] = lastReadings[(readingIndex + i) % NUM_READINGS];
        }
    }
    
    // Realiza a regressão linear
    float sum_x = 0.0f, sum_y = 0.0f, sum_x2 = 0.0f, sum_xy = 0.0f;
    for (int i = 0; i < n; i++) {
        float x = i * dt;         // Tempo correspondente ao i-ésimo ponto
        float y = orderedReadings[i];
        sum_x += x;
        sum_y += y;
        sum_x2 += x * x;
        sum_xy += x * y;
    }
    
    float numerator = n * sum_xy - sum_x * sum_y;
    float denominator = n * sum_x2 - sum_x * sum_x;
    
    if (denominator == 0) return 0.0f; // Evita divisão por zero
    
    slope = (numerator / denominator) * -1.0f;
    printf("Taxa de crescimento: %.2f cm/s\n", slope);
}
