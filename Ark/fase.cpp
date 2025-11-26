#include "fases.h"

GameConfig GetConfig(Dificuldade d) {
    GameConfig cfg;
    switch (d) {//atualiza as informações de acordo com a dificuldade
        case FACIL:
            cfg.velocidadeBola    = 4.0f;
            cfg.larguraPlataforma = 130.0f;
            cfg.vidaBloco         = 1;
            break;
        case MEDIO:
            cfg.velocidadeBola    = 5.5f;
            cfg.larguraPlataforma = 100.0f;
            cfg.vidaBloco         = 2;
            break;
        case DIFICIL:
            cfg.velocidadeBola    = 7.0f;
            cfg.larguraPlataforma = 70.0f;
            cfg.vidaBloco         = 3;
            break;
    }
    return cfg;
}

void ConfigurarFase(Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS], int faseAtual, const GameConfig& cfg) {

    int vidaBase = cfg.vidaBloco + (faseAtual - 1); // Fase 1: base, Fase 2: base+1, Fase 3: base+2

    for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
        for (int j = 0; j < QUANTIDADEBLOCOS; j++) {

            float posX = 10.0f + j * TAMANHOBLOCO;
            float posY = 10.0f + i * ALTURABLOCO;

            blocos[i][j].bloco.x      = posX;
            blocos[i][j].bloco.y      = posY;
            blocos[i][j].bloco.width  = TAMANHOBLOCO;
            blocos[i][j].bloco.height = ALTURABLOCO;
            blocos[i][j].ativo        = false;
            blocos[i][j].vida         = 0;
            blocos[i][j].cor          = GRAY;

            // FASE 1: duas linhas cheias simples 
            if (faseAtual == 1) {
                if (i < 2) {
                    blocos[i][j].ativo = true;
                    blocos[i][j].vida  = vidaBase;
                    if ((i + j) % 2 == 0)
                        blocos[i][j].cor = RED;//intercala entre vermelho e azul 
                    else
                        blocos[i][j].cor = BLUE;
                } else {
                    blocos[i][j].bloco.x = -200;
                    blocos[i][j].bloco.y = -200;
                }
            }
            //  FASE 2: padrão zigue-zague / xadrez
            else if (faseAtual == 2) {
                bool ativoAqui = false;
                if (i == 0 && j % 2 == 0)  ativoAqui = true;//ativa blocos nas colunas pares na linha 0
                if (i == 1 && j % 2 == 1)  ativoAqui = true;//ativa blocos nas coluna impares na linha 1
                if (i == 2 && (j % 3 != 1)) ativoAqui = true;//ativa a cada 3 coluna 1 fica vazia 

                if (ativoAqui) {
                    blocos[i][j].ativo = true;
                    blocos[i][j].vida  = vidaBase;
                    if ((i + j) % 2 == 0)
                        blocos[i][j].cor = GREEN;//intercala entre verde e laranja 
                    else
                        blocos[i][j].cor = ORANGE;
                } else {
                    blocos[i][j].bloco.x = -200;
                    blocos[i][j].bloco.y = -200;
                }
            }
            //FASE 3: pirâmide 
            else if (faseAtual == 3) {
                int meio = QUANTIDADEBLOCOS / 2; // 6
                bool ativoAqui = false;

                if (i == 0 && (j >= meio - 1 && j <= meio + 1)) ativoAqui = true;
                if (i == 1 && (j >= meio - 2 && j <= meio + 2)) ativoAqui = true;
                if (i == 2 && (j >= meio - 3 && j <= meio + 3)) ativoAqui = true;//ativa meio-3 ate meio+3

                if (ativoAqui) {
                    blocos[i][j].ativo = true;
                    blocos[i][j].vida  = vidaBase;

                    if (i == 0)      blocos[i][j].cor = PURPLE;
                    else if (i == 1) blocos[i][j].cor = GOLD;
                    else             blocos[i][j].cor = DARKPURPLE;
                } else {
                    blocos[i][j].bloco.x = -200;
                    blocos[i][j].bloco.y = -200;
                }
            }
        }
    }
}