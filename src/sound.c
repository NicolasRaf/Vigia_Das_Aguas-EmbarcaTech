#include "sound.h"

// Definição das notas
const int e5 = 659;
const int c4 = 261;
const int a5 = 880;
const int c5 = 523;
const int f5 = 698;
const int g5 = 784;
const int b4 = 466;

int volume = 30; // Volume padrão (não usado diretamente na reprodução)
bool isPlaying = false;
int currentNoteIndex = 0;
int noteCount = 0;
const int *currentNotes;
int noteDuration = 200;
int currentVolume = 30;  // Volume usado para a reprodução atual

// Inicializa os alto-falantes e o PWM
void initSound() {
    // Configura os pinos dos alto-falantes
    gpio_init(speakPin1);
    gpio_set_dir(speakPin1, GPIO_OUT);
    
    gpio_init(speakPin2);
    gpio_set_dir(speakPin2, GPIO_OUT);
    
    initPWM();
}

// Configuração do PWM no pino speakPin1
void initPWM() {
    gpio_set_function(speakPin1, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(speakPin1);
    // Usando um divisor fixo para garantir que o wrap caiba em uint16_t (valor ~62500)
    pwm_set_wrap(slice, 125000000 / 2000);
    pwm_set_enabled(slice, true);
}

// Ajusta a frequência do PWM e aplica um controle de volume usando escala logarítmica
void setPWMFrequency(int frequency) {
    uint slice = pwm_gpio_to_slice_num(speakPin1);
    if (frequency == 0) {
        pwm_set_gpio_level(speakPin1, 0);
        return;
    }
    // Calcula o wrap com base na frequência desejada
    uint32_t wrap = 125000000 / frequency;
    pwm_set_wrap(slice, wrap);
    
    // Controle de volume logarítmico para uma progressão mais natural:
    float duty_factor = log10(currentVolume + 1) / log10(101); // currentVolume de 0 a 100
    uint32_t level = (uint32_t)(wrap * duty_factor);
    pwm_set_gpio_level(speakPin1, level);
}

// Callback do timer que controla a reprodução assíncrona do som
bool soundCallback(struct repeating_timer *t) {
    if (currentNoteIndex >= noteCount) {
        isPlaying = false;
        setPWMFrequency(0);  // Para o PWM
        return false; // Cancela o timer
    }
    
    // Ajusta a frequência de saída conforme a nota atual
    setPWMFrequency(currentNotes[currentNoteIndex]);
    currentNoteIndex++;
    return true;
}

// Função para iniciar a reprodução de um som
void playSound(const int *notes, int count, int duration, int vol) {
    if (isPlaying) return; // Impede sobreposição
    
    currentNotes = notes;
    noteCount = count;
    noteDuration = duration;
    currentNoteIndex = 0;
    isPlaying = true;
    currentVolume = vol; // Define o volume para essa reprodução
    
    static struct repeating_timer timer;
    add_repeating_timer_ms(noteDuration, soundCallback, NULL, &timer);
}

// Sons pré-definidos

// Som de confirmação (usa volume padrão, por exemplo, 30)
void playConfirmSound() {
    static const int confirmNotes[] = {c5, e5, g5, c5};
    playSound(confirmNotes, 4, 200, 30);
}

// Som de erro (usa volume padrão)
void playErrorSound() {
    static const int errorNotes[] = {c4, c4 + 10, c4 - 10};
    playSound(errorNotes, 3, 150, 30);
}

// Som de alerta médio (usa um volume intermediário, por exemplo, 50)
void playSoundMedium() {
    static const int mediumAlertNotes[] = {e5, f5, g5, e5, f5, g5};
    playSound(mediumAlertNotes, 6, 300, 25);
}

// Som de alerta forte (sirene) - usa volume máximo (100) para ficar bem audível
void playSoundStrong() {
    static const int strongAlertNotes[] = {c4, b4, f5, c4, b4, f5};
    playSound(strongAlertNotes, 6, 50, 200);
}