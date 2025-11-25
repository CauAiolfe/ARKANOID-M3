#include "itens.h"
#include "raylib.h"

Item itens[MAX_ITENS];

void ClearItems() {
    for (int i = 0; i < MAX_ITENS; i++) {
        itens[i].ativo = false;
    }
}

void TrySpawnItem(Rectangle origem) {
    int chance = GetRandomValue(0, 99);
    if (chance > 30) return; // 30% de chance

    for (int i = 0; i < MAX_ITENS; i++) {
        if (!itens[i].ativo) {
            itens[i].ativo  = true;
            itens[i].pos    = { origem.x + origem.width / 2.0f,
                                origem.y + origem.height / 2.0f };
            itens[i].radius = 8.0f;
            itens[i].speed  = 3.0f;

            int tipoRand  = GetRandomValue(0, 3);
            itens[i].tipo = (ItemTipo)tipoRand;

            switch (itens[i].tipo) {
                case ITEM_VIDA:
                    itens[i].cor = GREEN;
                    break;
                case ITEM_PAD_UP:
                    itens[i].cor = BLUE;
                    break;
                case ITEM_PAD_DOWN:
                    itens[i].cor = RED;
                    break;
                case ITEM_SCORE:
                    itens[i].cor = GOLD;
                    break;
            }
            break;
        }
    }
}

void AtualizarItens(float screenHeight,
                    Rectangle& paddleRect,
                    int& vidas,
                    Vector2& plataformaPosicao,
                    Vector2& plataformaTamanho,
                    Vector2& plataformaPosicaoFim,
                    int screenWidth,
                    int& scoreAtual) {

    for (int k = 0; k < MAX_ITENS; k++) {
        if (!itens[k].ativo) continue;

        itens[k].pos.y += itens[k].speed;

        // Saiu da tela
        if (itens[k].pos.y - itens[k].radius > screenHeight) {
            itens[k].ativo = false;
            continue;
        }

        // Colisão com o paddle
        if (CheckCollisionCircleRec(itens[k].pos, itens[k].radius, paddleRect)) {

            switch (itens[k].tipo) {
                case ITEM_VIDA:
                    vidas++;
                    break;

                case ITEM_PAD_UP:
                    plataformaTamanho.x += 20.0f;
                    if (plataformaTamanho.x > 220.0f)
                        plataformaTamanho.x = 220.0f;
                    if (plataformaPosicao.x > screenWidth - (plataformaTamanho.x + 10))
                        plataformaPosicao.x = screenWidth - (plataformaTamanho.x + 10);
                    plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
                    break;

                case ITEM_PAD_DOWN:
                    plataformaTamanho.x -= 20.0f;
                    if (plataformaTamanho.x < 40.0f)
                        plataformaTamanho.x = 40.0f;
                    if (plataformaPosicao.x > screenWidth - (plataformaTamanho.x + 10))
                        plataformaPosicao.x = screenWidth - (plataformaTamanho.x + 10);
                    plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
                    break;

                case ITEM_SCORE: {
                    int efeito = GetRandomValue(0, 2);
                    if (efeito == 0) {
                        scoreAtual += 200;    // pontos extras
                    } else if (efeito == 1) {
                        scoreAtual *= 2;      // multiplicador
                    } else {
                        scoreAtual = -scoreAtual; // inversor
                    }
                    break;
                }
            }

            itens[k].ativo = false;

            // Atualiza o retângulo do paddle
            paddleRect.x      = plataformaPosicao.x;
            paddleRect.y      = plataformaPosicao.y;
            paddleRect.width  = plataformaTamanho.x;
            paddleRect.height = plataformaTamanho.y;
        }
    }
}