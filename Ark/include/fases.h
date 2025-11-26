#ifndef FASE_H
#define FASE_H

#include "raylib.h"

// CONSTANTES DO JOGO (usadas em várias partes)
#define QUANTIDADEBLOCOS      12
#define TAMANHOBLOCO          65.0f
#define ALTURABLOCO           50.0f
#define QUANTIDADELINHASBLOCOS 3
#define SPEEDBOLA             4.0f
#define TAMANHOBOLA           10.0f

// Enum de dificuldade
enum Dificuldade { FACIL = 0, MEDIO, DIFICIL };

// Blocos das fases
struct Blocos {
    Rectangle bloco;
    Color     cor;
    bool      ativo;
    int       vida;
};

// Configuração geral conforme dificuldade
struct GameConfig {
    float velocidadeBola;
    float larguraPlataforma;
    int   vidaBloco;
};

GameConfig GetConfig(Dificuldade d);

// Monta o layout de blocos da fase atual
void ConfigurarFase(Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS], int faseAtual, const GameConfig& cfg);

#endif