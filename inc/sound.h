#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "display.h"

// Definindo os pinos do alto-falante
#define speakPin1 21
#define speakPin2 10

// Definição das frequências das notas
extern const int e5;    // 659 Hz
extern const int c4;    // 261 Hz
extern const int a5;    // 880 Hz
extern const int c5;    // 523 Hz
extern const int f5;    // 698 Hz
extern const int g5;    // 784 Hz
extern const int b4;    // 466 Hz
extern const int c5s;   // 554 Hz

// Variáveis globais do sistema de som
extern int volume;           // Volume percentual (ex: 50 para 50%)
extern bool isPlaying;       // Indica se um som está em reprodução
extern int currentNoteIndex; // Índice da nota atual
extern int noteCount;        // Número total de notas do som atual
extern const int *currentNotes; // Ponteiro para o array de notas do som atual
extern int noteDuration;     // Duração de cada nota em milissegundos

// Inicia o sistema de som
void initSound();

// Inicializa o PWM no pino indicado
void initPWM();

// Ajusta a frequência do PWM no pino indicado, controlando também o volume
void setPWMFrequency(int frequency);

// Callback do timer para controlar a reprodução do som
bool soundCallback(struct repeating_timer *t);

// Inicia a reprodução de um som definido por um array de notas, quantidade e duração
void playSound(const int *notes, int count, int duration, int vol);

// Função para tocar um som de confirmação
void playConfirmSound();

// Função para tocar um som de erro
void playErrorSound();

// Função para tocar um som de alerta médio
void playSoundMedium();

// Função para tocar um som de alerta forte (sirene)
void playSoundStrong();

#endif // SOUND_H
