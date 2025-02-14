#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "display.h"

// Definindo os pinos do alto-falante
#define speakPin1 21
#define speakPin2 10

// Função de configuração inicial
void setup();

// Função para tocar uma nota com volume
void playNoteWithVolume(int frequency, int duration, int volumePercent);

// Função de callback para tocar a música
bool musicCallback(repeating_timer_t *timer);

// Função para iniciar a música
void playMusic();

// Função para tocar um som de confirmação
void playConfirmSound();

// Função para tocar um som de erro
void playErrorSound();

// Função para imprimir 
void finishedMusic();

#endif // MUSIC_H
