#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>

#include "fases.h"
#include "itens.h"
#include "ranking.h"

using namespace std;

// Fases
int  faseAtual          = 1;
int  faseMensagem       = 1;
const int TOTAL_FASES   = 3;
bool telaTransicaoFase  = false;

// Plataforma (player)
Vector2 plataformaPosicao     = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim  = {450.0f, 550.0f};
Vector2 plataformaTamanho     = {100.0f, 20.0f};
Color   cor                   = MAROON;

// Pelota
Vector2 bolaPosicao        = {400.0f, 539.0f};

// Movimentação da bola
int   direcao         = -1;
int   diagonal        = 1;
float bolaVelocidadeY = SPEEDBOLA;
float bolaVelocidadeX = SPEEDBOLA;
bool  moveDiagonal    = false;

// Estados do jogo
bool gameOver        = false;
bool telaNome        = false;
bool telaDificuldade = true;

// Pontuação e vidas
int blocosQuebrados = 0;
int framesJogando   = 0;
int pontuacaoFinal  = 0;
int scoreAtual      = 0;  // score em tempo real (pontos + itens)
int vidas           = 3;  // vidas cumulativas

// Nome do jogador
string nomeJogador = "";

// Dificuldade atual
Dificuldade dificuldadeAtual = FACIL;

// Reinicia o jogo (posições, blocos). Se resetTotais = true, zera pontuações e volta pra fase 1.
void ResetGame(Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS], bool resetTotais) {
    if (resetTotais) {
        blocosQuebrados = 0;
        framesJogando   = 0;
        pontuacaoFinal  = 0;
        scoreAtual      = 0;
        vidas           = 3;
        faseAtual       = 1;
        faseMensagem    = 1;
    }

    GameConfig cfg = GetConfig(dificuldadeAtual);

    // Tamanho da plataforma conforme dificuldade
    plataformaTamanho = { cfg.larguraPlataforma, 20.0f };

    plataformaPosicao    = {350.0f, 550.0f};
    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};

    bolaPosicao      = {400.0f, 539.0f};
    direcao          = -1;
    diagonal         = 1;
    bolaVelocidadeY  = cfg.velocidadeBola;
    bolaVelocidadeX  = cfg.velocidadeBola;
    moveDiagonal     = false;

    ClearItems();                // do módulo de itens
    ConfigurarFase(blocos,       // do módulo de fases
                   faseAtual,
                   cfg);
}

int main() {
    const int screenWidth  = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Arkanoid - Raylib");
    SetTargetFPS(60);

    // ===== FUNDO DE ESTRELAS =====
    static Vector2 estrelas[200];
    static bool estrelasInit = false;

    // Não deixar ESC fechar automaticamente
    SetExitKey(0);

    Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS];
    ResetGame(blocos, true);

    while (true) {

        if (WindowShouldClose()) break;
        if (IsKeyPressed(KEY_ESCAPE)) break;

        // Inicializa as estrelas uma vez
        if (!estrelasInit) {
            for (int i = 0; i < 200; i++) {
                estrelas[i] = {
                    (float)GetRandomValue(0, screenWidth),
                    (float)GetRandomValue(0, screenHeight)
                };
            }
            estrelasInit = true;
        }

        // ===================== TELA DE DIFICULDADE =====================
        if (telaDificuldade) {

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                dificuldadeAtual = (Dificuldade)((((int)dificuldadeAtual) + 2) % 3); // sobe
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                dificuldadeAtual = (Dificuldade)((((int)dificuldadeAtual) + 1) % 3); // desce
            }

            if (IsKeyPressed(KEY_ENTER)) {
                telaDificuldade = false;
                telaNome        = true;
                ResetGame(blocos, true);  // novo jogo
            }

            BeginDrawing();
            ClearBackground(BLACK);

            // Fundo de estrelas na tela de dificuldade também
            for (int i = 0; i < 200; i++) {
                estrelas[i].y += 1.5f;
                if (estrelas[i].y > screenHeight) {
                    estrelas[i].y = 0;
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);
            }

            DrawText("SELECIONE A DIFICULDADE", screenWidth/2 - 210, 120, 30, RAYWHITE);
            DrawText("Use SETAS CIMA/BAIXO ou W/S para trocar", screenWidth/2 - 260, 170, 20, GRAY);
            DrawText("ENTER para confirmar", screenWidth/2 - 130, 200, 20, GRAY);

            const char* textoFacil   = "FACIL";
            const char* textoMedio   = "MEDIO";
            const char* textoDificil = "DIFICIL";

            Color corFacil   = (dificuldadeAtual == FACIL)   ? RED   : DARKGRAY;
            Color corMedio   = (dificuldadeAtual == MEDIO)   ? RED   : DARKGRAY;
            Color corDificil = (dificuldadeAtual == DIFICIL) ? RED   : DARKGRAY;

            DrawText(textoFacil,   screenWidth/2 - 50, 260, 30, corFacil);
            DrawText(textoMedio,   screenWidth/2 - 50, 310, 30, corMedio);
            DrawText(textoDificil, screenWidth/2 - 60, 360, 30, corDificil);

            // ===== TOP 5 RANKING =====
            vector<Score> top5 = LerRankingTop5();
            DrawText("TOP 5 RANKING:", 40, 410, 20, YELLOW);

            for (int i = 0; i < (int)top5.size(); i++) {
                DrawText(
                    TextFormat("%d. %s - %d",
                               i + 1,
                               top5[i].nome.c_str(),
                               top5[i].pontos),
                    40,
                    440 + i * 25,
                    18,
                    RAYWHITE
                );
            }

            EndDrawing();
            continue;
        }

        // ===================== TELA DE NOME =====================
        if (telaNome) {
            int key = GetKeyPressed();
            while (key > 0) {
                if (key == KEY_BACKSPACE && !nomeJogador.empty()) {
                    nomeJogador.pop_back();
                }
                else if (key >= 32 && key <= 125) {
                    nomeJogador.push_back((char)key);
                }
                key = GetKeyPressed();
            }

            if (IsKeyPressed(KEY_ENTER) && !nomeJogador.empty()) {
                telaNome = false;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            // Fundo de estrelas aqui também
            for (int i = 0; i < 200; i++) {
                estrelas[i].y += 1.5f;
                if (estrelas[i].y > screenHeight) {
                    estrelas[i].y = 0;
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);
            }

            DrawText("Digite seu nome:", screenWidth/2 - 150, 200, 30, RAYWHITE);

            DrawRectangle(screenWidth/2 - 150, 250, 300, 50, DARKGRAY);
            DrawText(nomeJogador.c_str(), screenWidth/2 - 140, 260, 30, SKYBLUE);

            DrawText("Pressione ENTER para continuar",
                     screenWidth/2 - 160, 330, 20, GRAY);

            EndDrawing();
            continue;
        }

        // ===================== TELA DE TRANSIÇÃO DE FASE =====================
        if (telaTransicaoFase) {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) {
                estrelas[i].y += 1.5f;
                if (estrelas[i].y > screenHeight) {
                    estrelas[i].y = 0;
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);
            }

            DrawText(TextFormat("Fase %d completa!", faseMensagem - 1),
                     screenWidth/2 - 170, 200, 30, LIME);
            DrawText(TextFormat("Proxima fase: %d", faseMensagem),
                     screenWidth/2 - 130, 240, 25, RAYWHITE);
            DrawText("Pressione ENTER para continuar",
                     screenWidth/2 - 170, 300, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                telaTransicaoFase = false;
            }

            EndDrawing();
            continue;
        }

        // ==================== LÓGICA DO JOGO ====================
        if (!gameOver && !telaTransicaoFase) {

            framesJogando++;

            if (IsKeyDown(KEY_RIGHT) && plataformaPosicao.x < screenWidth - (plataformaTamanho.x + 10)) {
                plataformaPosicao.x    += 10.0f;
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
            }
            if (IsKeyDown(KEY_LEFT) && plataformaPosicao.x > 10) {
                plataformaPosicao.x    -= 10.0f;
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         plataformaPosicao, plataformaPosicaoFim)) {
                direcao *= -1;

                if (bolaPosicao.x - plataformaPosicao.x < plataformaTamanho.x / 2.0f) {
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((plataformaTamanho.x/2.0f - (bolaPosicao.x - plataformaPosicao.x)) /
                                       (plataformaTamanho.x/2.0f));
                    moveDiagonal    = true;
                    diagonal        = -1;
                }
                else if (bolaPosicao.x - plataformaPosicao.x > plataformaTamanho.x / 2.0f) {
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((bolaPosicao.x - plataformaPosicao.x - plataformaTamanho.x/2.0f) /
                                       (plataformaTamanho.x/2.0f));
                    moveDiagonal    = true;
                    diagonal        = 1;
                }
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {0.0f, 10.0f}, {800.0f, 10.0f}))
                direcao *= -1;
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {10.0f, 0.0f}, {10.0f, 600.0f}))
                diagonal = 1;
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {790.0f, 0.0f}, {790.0f, 600.0f}))
                diagonal = -1;

            bolaPosicao.y += bolaVelocidadeY * direcao;
            if (moveDiagonal)
                bolaPosicao.x += bolaVelocidadeX * diagonal;

            // Se a bola cair fora da tela -> perde vida
            if (bolaPosicao.y - TAMANHOBOLA > screenHeight) {
                vidas--;
                if (vidas <= 0) {
                    gameOver = true;
                    int pontosTempo  = framesJogando;
                    int pontosBlocos = scoreAtual;
                    pontuacaoFinal   = pontosTempo + pontosBlocos;
                    SalvarRanking(nomeJogador, pontuacaoFinal);
                } else {
                    plataformaPosicao    = {350.0f, 550.0f};
                    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};
                    bolaPosicao          = {400.0f, 539.0f};
                    direcao              = -1;
                    diagonal             = 1;
                    moveDiagonal         = false;
                }
            }

            Vector2 futuraX = { bolaPosicao.x + (bolaVelocidadeX * diagonal), bolaPosicao.y };
            Vector2 futuraY = { bolaPosicao.x, bolaPosicao.y + (bolaVelocidadeY * direcao) };

            bool houveColisao = false;

            for (int i = 0; i < QUANTIDADELINHASBLOCOS && !houveColisao; i++) {
                for (int j = 0; j < QUANTIDADEBLOCOS && !houveColisao; j++) {

                    if (!blocos[i][j].ativo) continue;

                    Rectangle r = blocos[i][j].bloco;

                    bool colisaoX = CheckCollisionCircleRec(futuraX, TAMANHOBOLA, r);
                    bool colisaoY = CheckCollisionCircleRec(futuraY, TAMANHOBOLA, r);

                    if (colisaoX || colisaoY) {
                        if (colisaoX && !colisaoY) {
                            diagonal = -diagonal;
                        }
                        else if (colisaoY && !colisaoX) {
                            direcao = -direcao;
                        }
                        else {
                            if (fabs(bolaPosicao.x - (r.x + r.width/2)) >
                                fabs(bolaPosicao.y - (r.y + r.height/2)))
                                diagonal = -diagonal;
                            else
                                direcao = -direcao;
                        }

                        blocos[i][j].vida--;
                        if (blocos[i][j].vida <= 0) {
                            blocos[i][j].ativo   = false;
                            blocos[i][j].bloco.x = -200;
                            blocos[i][j].bloco.y = -200;
                            blocosQuebrados++;
                            scoreAtual += 50;
                            TrySpawnItem(r); // módulo de itens
                        } else {
                            if (blocos[i][j].vida == 2)
                                blocos[i][j].cor = ORANGE;
                            else if (blocos[i][j].vida == 1)
                                blocos[i][j].cor = YELLOW;
                        }

                        houveColisao = true;
                    }
                }
            }

            // Atualiza itens especiais (queda + coleta)
            Rectangle paddleRect = {
                plataformaPosicao.x,
                plataformaPosicao.y,
                plataformaTamanho.x,
                plataformaTamanho.y
            };

            AtualizarItens((float)screenHeight,
                           paddleRect,
                           vidas,
                           plataformaPosicao,
                           plataformaTamanho,
                           plataformaPosicaoFim,
                           screenWidth,
                           scoreAtual);

            // Verifica se acabou a fase
            if (!gameOver) {
                int ativos = 0;
                for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
                    for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
                        if (blocos[i][j].ativo) ativos++;
                    }
                }

                if (ativos == 0) {
                    if (faseAtual < TOTAL_FASES) {
                        faseAtual++;
                        faseMensagem      = faseAtual;
                        ResetGame(blocos, false);
                        telaTransicaoFase = true;
                    } else {
                        gameOver = true;
                        int pontosTempo  = framesJogando;
                        int pontosBlocos = scoreAtual;
                        pontuacaoFinal   = pontosTempo + pontosBlocos;
                        SalvarRanking(nomeJogador, pontuacaoFinal);
                    }
                }
            }
        }
        else if (gameOver) {
            if (IsKeyPressed(KEY_ENTER)) {
                gameOver        = false;
                telaDificuldade = true;
                telaNome        = false;
                nomeJogador.clear();
                ResetGame(blocos, true);
            }
        }

        // ==================== DESENHO ====================
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < 200; i++) {
            estrelas[i].y += 1.5f;
            if (estrelas[i].y > screenHeight) {
                estrelas[i].y = 0;
                estrelas[i].x = (float)GetRandomValue(0, screenWidth);
            }
            DrawPixelV(estrelas[i], LIGHTGRAY);
        }

        if (!gameOver) {
            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()),
                     20, 20, 25, RAYWHITE);

            const char* textoDif;
            switch (dificuldadeAtual) {
                case FACIL:   textoDif = "FACIL";   break;
                case MEDIO:   textoDif = "MEDIO";   break;
                case DIFICIL: textoDif = "DIFICIL"; break;
            }
            DrawText(TextFormat("Dificuldade: %s", textoDif), 20, 50, 20, GRAY);
            DrawText(TextFormat("Fase: %d/%d", faseAtual, TOTAL_FASES),
                     20, 80, 20, GRAY);

            DrawText(TextFormat("Vidas: %d", vidas),
                     20, 110, 20, RED);

            DrawText(TextFormat("Blocos quebrados: %d", blocosQuebrados),
                     550, 20, 20, RAYWHITE);
            DrawText(TextFormat("Tempo: %.1fs", framesJogando/60.0f),
                     550, 50, 20, RAYWHITE);

            DrawText(TextFormat("Score: %d", scoreAtual),
                     550, 80, 20, SKYBLUE);

            for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
                for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
                    if (!blocos[i][j].ativo) continue;

                    DrawRectangle(blocos[i][j].bloco.x,
                                  blocos[i][j].bloco.y,
                                  blocos[i][j].bloco.width,
                                  blocos[i][j].bloco.height,
                                  blocos[i][j].cor);
                }
            }

            // Desenha itens especiais
            for (int k = 0; k < MAX_ITENS; k++) {
                if (!itens[k].ativo) continue;
                DrawCircleV(itens[k].pos, itens[k].radius, itens[k].cor);
            }

            DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);
            DrawCircleV(bolaPosicao, TAMANHOBOLA, GREEN);
        }
        else {
            DrawText("GAME OVER", 250, 120, 60, RED);

            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()),
                     300, 200, 25, RAYWHITE);

            DrawText(TextFormat("Pontuacao final: %d", pontuacaoFinal),
                     250, 260, 30, LIME);

            DrawText("ENTER = Voltar ao menu", 260, 330, 20, GRAY);
            DrawText("ESC = Sair",              340, 360, 20, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
