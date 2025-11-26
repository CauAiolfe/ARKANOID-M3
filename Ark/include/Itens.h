#ifndef ITENS_H
#define ITENS_H

#include "raylib.h"

// Tipos de itens especiais
enum ItemTipo {
    ITEM_VIDA = 0,
    ITEM_PAD_UP,
    ITEM_PAD_DOWN,
    ITEM_SCORE
};

struct Item {
    Vector2 pos;
    float   radius;
    float   speed;
    ItemTipo tipo;
    Color   cor;
    bool    ativo;
};

const int MAX_ITENS = 30;

// "extern" é usado para mostrar que não foi criado aqui foi trago de itens.cpp
extern Item itens[MAX_ITENS];

// Zera todos os itens
void ClearItems();

// Chamada ao quebrar um bloco -> tenta spawnar item
void SpawnItem(Rectangle origem);

// Atualiza queda, colisão com o paddle e aplica efeitos
void AtualizarItens(float screenHeight, Rectangle& paddleRect, int& vidas, Vector2& plataformaPosicao, Vector2& plataformaTamanho, Vector2& plataformaPosicaoFim, int screenWidth, int& scoreAtual);

#endif