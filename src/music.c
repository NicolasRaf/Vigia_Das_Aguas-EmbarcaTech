#include "music.h"

// Definindo as frequências das notas
const int e5 = 659;  // E5
const int c4 = 261;  // C4
const int a5 = 880;
const int c5 = 523;
const int f5 = 698;
const int g5 = 784;
const int b4 = 466;
const int c5s = 554;

int volume = 2; // Volume percentual
int currentSpeaker = speakPin1;

// Estrutura para armazenar o estado da música
typedef struct {
    int noteIndex;
    int duration;
    bool playing;
    int notes[17];
} MusicState;

// Estado inicial da música
MusicState musicState = {
    .noteIndex = 0,
    .duration = 200,
    .playing = false,
    .notes = {a5, c5, f5, c5, a5, c5, f5, c5, a5, c5, f5, c5, b4, c5s, f5, g5, a5}
};

// Função de configuração inicial
void setup() {
    static bool isSetup = false;
    if (isSetup) return;

    gpio_init(speakPin1);
    gpio_set_dir(speakPin1, GPIO_OUT);
    gpio_init(speakPin2);
    gpio_set_dir(speakPin2, GPIO_OUT);
    stdio_init_all();
    isSetup = true;
}

// Função para tocar uma nota com volume
void playNoteWithVolume(int frequency, int duration, int volumePercent) {
    float volume = volumePercent / 100.0;
    int period = 1000000 / frequency; // Período da onda em microssegundos
    int pulseWidth = period * volume;

    absolute_time_t startTime = get_absolute_time();
    while (absolute_time_diff_us(startTime, get_absolute_time()) < duration * 1000) {
        gpio_put(currentSpeaker, 1);
        busy_wait_us_32(pulseWidth);
        gpio_put(currentSpeaker, 0);
        busy_wait_us_32(period - pulseWidth);
    }

    // Alterna entre os alto-falantes
    currentSpeaker = (currentSpeaker == speakPin1) ? speakPin2 : speakPin1;
}

// Função de callback para tocar a música
bool musicCallback(repeating_timer_t *timer) {
    if (!musicState.playing || musicState.noteIndex >= 17) {
        musicState.playing = false;
        finishedMusic();
        return false; // Cancela o timer
    }

    int note = musicState.notes[musicState.noteIndex];
    playNoteWithVolume(note, musicState.duration, volume);
    musicState.noteIndex++;
    return true; // Continua o timer
}

// Função para iniciar a música
void playMusic() {
    setup();

    if (musicState.playing) {
        return; // Já está tocando
    }

    printf("Tocando Musica...\n");

    musicState.noteIndex = 0;
    musicState.playing = true;

    static repeating_timer_t timer;
    add_repeating_timer_ms(musicState.duration, musicCallback, NULL, &timer);
}

// Função para tocar um som de confirmação
void playConfirmSound() {
    setup();
    int confirmNotes[] = {c5, e5, g5, c5};
    int confirmDuration = 200;

    for (int i = 0; i < 4; i++) {
        playNoteWithVolume(confirmNotes[i], confirmDuration, volume);
        busy_wait_ms(50);
    }
}

// Função para tocar um som de erro
void playErrorSound() {
    setup();

    int errorNotes[] = {c4, c4 + 10, c4 - 10};
    int errorDuration = 150;

    for (int i = 0; i < 3; i++) {
        playNoteWithVolume(errorNotes[i], errorDuration, volume);
        busy_wait_ms(50);
    }
}

void finishedMusic() {
    printf("Musica Finalizada");
    showText("Musica Finalizada!", 0, 0, 1, true);
}