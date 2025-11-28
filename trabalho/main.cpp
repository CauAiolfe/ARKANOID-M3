#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>
#include "fases.h"
#include "itens.h"
#include "ranking.h"
using namespace std;


enum Tela {
    TELA_MENU,
    TELA_DIFICULDADE,
    TELA_RANKING,
    TELA_NOME,
    TELA_JOGO,
    TELA_TRANSICAO,
    TELA_GAMEOVER
};

Tela telaAtual = TELA_MENU;


int faseAtual = 1;
int faseMensagem = 1;
const int TOTAL_FASES = 3;
bool telaTransicaoFase = false;

Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim = {450.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};
Color cor = MAROON;

Vector2 bolaPosicao = {400.0f, 539.0f};
int direcao = -1;
int diagonal = 1;
float bolaVelocidadeY = SPEEDBOLA;
float bolaVelocidadeX = SPEEDBOLA;
bool moveDiagonal = false;

bool gameOver = false;
bool telaNome = false;
bool telaDificuldade = false;

int blocosQuebrados = 0;
int framesJogando = 0;
int pontuacaoFinal = 0;
int scoreAtual = 0;
int vidas = 3;

string nomeJogador = "";

Dificuldade dificuldadeAtual = FACIL;

void ResetGame(Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS], bool resetTotais) {
    if (resetTotais) {
        blocosQuebrados = 0;
        framesJogando = 0;
        pontuacaoFinal = 0;
        scoreAtual = 0;
        vidas = 3;
        faseAtual = 1;
        faseMensagem = 1;
    }

    GameConfig cfg = GetConfig(dificuldadeAtual);

    plataformaTamanho = {cfg.larguraPlataforma, 20.0f};
    plataformaPosicao = {350.0f, 550.0f};
    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};

    bolaPosicao = {400.0f, 539.0f};
    direcao = -1;
    diagonal = 1;
    bolaVelocidadeY = cfg.velocidadeBola;
    bolaVelocidadeX = cfg.velocidadeBola;
    moveDiagonal = false;

    ClearItems();
    ConfigurarFase(blocos, faseAtual, cfg);
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Arkanoid — Menu Completo");
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice(); // inicia o som 

    Sound somToque = LoadSound("assents/somBatida.wav");

    Texture2D texBloco = LoadTexture("assents/bloco.png");
    Texture2D texBola  = LoadTexture("assents/bola.png");
    Texture2D texVida  = LoadTexture("assents/vidaextra.png");
    Texture2D texPadUp = LoadTexture("assents/aumentaBase.png");
    Texture2D texPadDown = LoadTexture("assents/diminiuBase.png");
    Texture2D texScore   = LoadTexture("assents/pontuacao.png");
    Texture2D texB1 = LoadTexture("assents/blocoB1.png");
    Texture2D texB2 = LoadTexture("assents/blocoB2.png");


    static Vector2 estrelas[200];
    static bool estrelasInit = false;

    Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS];
    ResetGame(blocos, true);

    // Inicialização do fundo estrelado
    for (int i = 0; i < 200; i++) {
        estrelas[i] = {(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)};
    }
    estrelasInit = true;

    while (!WindowShouldClose()) {

        
        // ANIMAÇÃO DO FUNDO DE ESTRELAS
        
        for (int i = 0; i < 200; i++) {
            estrelas[i].y += 1.5f;
            if (estrelas[i].y > screenHeight) {
                estrelas[i].y = 0;
                estrelas[i].x = (float)GetRandomValue(0, screenWidth);
            }
        }

        
        //  TELA 1 — MENU INICIAL (OPÇÃO A)
        
        if (telaAtual == TELA_MENU) {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText("A R K A N O I D", screenWidth/2 - 180, 80, 50, SKYBLUE);

            // Botões grandes
            Rectangle btnJogar = { screenWidth/2 - 150, 200, 300, 60 };
            Rectangle btnDificuldade = { screenWidth/2 - 150, 300, 300, 60 };
            Rectangle btnRanking = { screenWidth/2 - 150, 400, 300, 60 };

        
            Color hJogar = CheckCollisionPointRec(GetMousePosition(), btnJogar) ? RED : DARKGRAY;
            Color hDif   = CheckCollisionPointRec(GetMousePosition(), btnDificuldade) ? RED : DARKGRAY;
            Color hRank  = CheckCollisionPointRec(GetMousePosition(), btnRanking) ? RED : DARKGRAY;

            DrawRectangleRec(btnJogar, hJogar);
            DrawRectangleRec(btnDificuldade, hDif);
            DrawRectangleRec(btnRanking, hRank);

            DrawText("JOGAR", btnJogar.x + 90, btnJogar.y + 15, 30, WHITE);
            DrawText("DIFICULDADES", btnDificuldade.x + 40, btnDificuldade.y + 15, 30, WHITE);
            DrawText("RANKING", btnRanking.x + 90, btnRanking.y + 15, 30, WHITE);

            // Clique do mouse
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mp = GetMousePosition();

                if (CheckCollisionPointRec(mp, btnJogar)) {
                    telaAtual = TELA_NOME;
                }
                else if (CheckCollisionPointRec(mp, btnDificuldade)) {
                    telaAtual = TELA_DIFICULDADE;
                }
                else if (CheckCollisionPointRec(mp, btnRanking)) {
                    telaAtual = TELA_RANKING;
                }
            }

            EndDrawing();
            continue;
        }
        
        //  TELA 2 — SELEÇÃO DE DIFICULDADE
        
        if (telaAtual == TELA_DIFICULDADE) {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText("SELECIONE A DIFICULDADE", screenWidth/2 - 220, 120, 30, RAYWHITE);

            Rectangle btnF = { screenWidth/2 -150, 220, 300, 50 };
            Rectangle btnM = { screenWidth/2 -150, 290, 300, 50 };
            Rectangle btnD = { screenWidth/2 -150, 360, 300, 50 };

            Color cF = (dificuldadeAtual == FACIL)   ? RED : DARKGRAY;
            Color cM = (dificuldadeAtual == MEDIO)   ? RED : DARKGRAY;
            Color cD = (dificuldadeAtual == DIFICIL) ? RED : DARKGRAY;

            DrawRectangleRec(btnF, cF);
            DrawRectangleRec(btnM, cM);
            DrawRectangleRec(btnD, cD);

            DrawText("FACIL", btnF.x + 110, btnF.y + 10, 30, WHITE);
            DrawText("MEDIO", btnM.x + 105, btnM.y + 10, 30, WHITE);
            DrawText("DIFICIL", btnD.x + 90, btnD.y + 10, 30, WHITE);

            // clique
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mp = GetMousePosition();
                if (CheckCollisionPointRec(mp, btnF)) dificuldadeAtual = FACIL;
                if (CheckCollisionPointRec(mp, btnM)) dificuldadeAtual = MEDIO;
                if (CheckCollisionPointRec(mp, btnD)) dificuldadeAtual = DIFICIL;
            }

            // voltar com ESC
            if (IsKeyPressed(KEY_ESCAPE)) telaAtual = TELA_MENU;

            EndDrawing();
            continue;
        }

        
        //  TELA 3 — RANKING CENTRALIZADO
        
        if (telaAtual == TELA_RANKING) {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText("TOP 5 RANKING", screenWidth/2 - 150, 100, 40, YELLOW);

            vector<Score> top5 = LerRankingTop5();

            for (int i = 0; i < (int)top5.size(); i++) {
                DrawText(TextFormat("%d. %s - %d", i+1, top5[i].nome.c_str(), top5[i].pontos),
                         screenWidth/2 - 200, 200 + i*40, 30, RAYWHITE);
            }

            DrawText("ESC = Voltar", screenWidth/2 - 80, 500, 20, GRAY);

            if (IsKeyPressed(KEY_ESCAPE)) telaAtual = TELA_MENU;

            EndDrawing();
            continue;
        }

        
        //  TELA 4 — NOME DO JOGADOR
        
        if (telaAtual == TELA_NOME) {
            int key = GetKeyPressed();
            while (key > 0) {
                if (key == KEY_BACKSPACE && !nomeJogador.empty()) nomeJogador.pop_back();
                else if (key >= 32 && key <= 125) nomeJogador.push_back((char)key);
                key = GetKeyPressed();
            }

            if (IsKeyPressed(KEY_ENTER) && !nomeJogador.empty()) {
                telaAtual = TELA_JOGO;
                ResetGame(blocos, true);
            }

            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText("Digite seu nome:", screenWidth/2 - 150, 200, 30, WHITE);
            DrawRectangle(screenWidth/2 -150, 250, 300, 50, DARKGRAY);
            DrawText(nomeJogador.c_str(), screenWidth/2 -140, 260, 30, SKYBLUE);

            DrawText("ENTER para continuar", screenWidth/2 -140, 330, 20, GRAY);

            EndDrawing();
            continue;
        }

        
        //  TELA: JOGO (LÓGICA PRINCIPAL)
        
        if (telaAtual == TELA_JOGO) {
            // comportamentos equivalentes ao bloco original de lógica do jogo
            // Nota: mantivemos todas as variáveis e chamadas originais para não alterar a jogabilidade

            framesJogando++;

            if (IsKeyDown(KEY_RIGHT) && plataformaPosicao.x < screenWidth - (plataformaTamanho.x + 10)) {
                plataformaPosicao.x += 10.0f;
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
            }
            if (IsKeyDown(KEY_LEFT) && plataformaPosicao.x > 10) {
                plataformaPosicao.x -= 10.0f;
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         plataformaPosicao, plataformaPosicaoFim)){
            
                PlaySound(somToque);

                direcao *= -1;
                

                if (bolaPosicao.x - plataformaPosicao.x < plataformaTamanho.x / 2.0f){
                    
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((plataformaTamanho.x / 2.0f - (bolaPosicao.x - plataformaPosicao.x)) /
                                       (plataformaTamanho.x / 2.0f));
                    moveDiagonal = true;
                    diagonal = -1;
                    
                }
                else if (bolaPosicao.x - plataformaPosicao.x > plataformaTamanho.x / 2.0f){
                   
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((bolaPosicao.x - plataformaPosicao.x - plataformaTamanho.x / 2.0f) /
                                       (plataformaTamanho.x / 2.0f));
                    moveDiagonal = true;
                    diagonal = 1;
                    
                }
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {0.0f, 10.0f}, {800.0f, 10.0f}))
                                         
                direcao *= -1;
                PlaySound(somToque);
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {10.0f, 0.0f}, {10.0f, 600.0f}))
                                         
                diagonal = 1;
                PlaySound(somToque);
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {790.0f, 0.0f}, {790.0f, 600.0f}))
                diagonal = -1;
                PlaySound(somToque);

            bolaPosicao.y += bolaVelocidadeY * direcao;
            if (moveDiagonal)
                bolaPosicao.x += bolaVelocidadeX * diagonal;
                

            // se a bola cair fora da tela -> perde vida
            if (bolaPosicao.y - TAMANHOBOLA > screenHeight){
                vidas--;
                if (vidas <= 0) {
                    gameOver = true;
                    int pontosTempo = framesJogando;
                    int pontosBlocos = scoreAtual;
                    pontuacaoFinal = pontosTempo + pontosBlocos;
                    SalvarRanking(nomeJogador, pontuacaoFinal);
                    // muda para tela de game over
                    telaAtual = TELA_GAMEOVER;
                } else {
                    plataformaPosicao = {350.0f, 550.0f};
                    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};
                    bolaPosicao = {400.0f, 539.0f};
                    direcao = -1;
                    diagonal = 1;
                    moveDiagonal = false;
                }
            }

            Vector2 futuraX = {bolaPosicao.x + (bolaVelocidadeX * diagonal), bolaPosicao.y};
            Vector2 futuraY = {bolaPosicao.x, bolaPosicao.y + (bolaVelocidadeY * direcao)};

            bool houveColisao = false;

            for (int i = 0; i < QUANTIDADELINHASBLOCOS && !houveColisao; i++){
                for (int j = 0; j < QUANTIDADEBLOCOS && !houveColisao; j++){
                    if (!blocos[i][j].ativo) continue;

                    Rectangle r = blocos[i][j].bloco;

                    bool colisaoX = CheckCollisionCircleRec(futuraX, TAMANHOBOLA, r);
                    bool colisaoY = CheckCollisionCircleRec(futuraY, TAMANHOBOLA, r);
                    

                    if (colisaoX || colisaoY){
                        PlaySound(somToque);
                        if (colisaoX && !colisaoY){
                            diagonal = -diagonal;

                        }
                        else if (colisaoY && !colisaoX){
                            direcao = -direcao;
                        }
                        else {
                            if (fabs(bolaPosicao.x - (r.x + r.width / 2)) >
                                fabs(bolaPosicao.y - (r.y + r.height / 2)))
                                diagonal = -diagonal;
                            else
                                direcao = -direcao;
                        }

                        blocos[i][j].vida--;
                        if (blocos[i][j].vida <= 0){
                            blocos[i][j].ativo = false;
                            blocos[i][j].bloco.x = -200;
                            blocos[i][j].bloco.y = -200;
                            blocosQuebrados++;
                            scoreAtual += 50;
                            SpawnItem(r);
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

            Rectangle paddleRect = { plataformaPosicao.x, plataformaPosicao.y, plataformaTamanho.x, plataformaTamanho.y };

            AtualizarItens((float)screenHeight,
                           paddleRect,
                           vidas,
                           plataformaPosicao,
                           plataformaTamanho,
                           plataformaPosicaoFim,
                           screenWidth,
                           scoreAtual);

            // verifica se acabou a fase
            if (!gameOver){
                int ativos = 0;
                for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++){
                    for (int j = 0; j < QUANTIDADEBLOCOS; j++){
                        if (blocos[i][j].ativo) ativos++;
                    }
                }

                if (ativos == 0){
                    if (faseAtual < TOTAL_FASES){
                        faseAtual++;
                        faseMensagem = faseAtual;
                        ResetGame(blocos, false);
                        telaTransicaoFase = true;
                        telaAtual = TELA_TRANSICAO;
                    } else {
                        gameOver = true;
                        int pontosTempo = framesJogando;
                        int pontosBlocos = scoreAtual;
                        pontuacaoFinal = pontosTempo + pontosBlocos;
                        SalvarRanking(nomeJogador, pontuacaoFinal);
                        telaAtual = TELA_GAMEOVER;
                    }
                }
            }

            // desenho da tela de jogo (apenas quando ainda em TELA_JOGO)
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()), 20, 20, 25, RAYWHITE);

            const char* textoDif;
            switch (dificuldadeAtual){ case FACIL: textoDif = "FACIL"; break; case MEDIO: textoDif = "MEDIO"; break; case DIFICIL: textoDif = "DIFICIL"; break; }

            for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++){
                for (int j = 0; j < QUANTIDADEBLOCOS; j++){
                    if (!blocos[i][j].ativo) continue;
                    float scale;

                    if (blocos[i][j].vida == blocos[i][j].vidaMax) {
                    
                        scale = blocos[i][j].bloco.width / (float)texBloco.width;
                        DrawTextureEx(
                            texBloco,
                            { blocos[i][j].bloco.x, blocos[i][j].bloco.y },
                            0.0f,
                            scale,
                            WHITE
                        );
                    }
                    else if (blocos[i][j].vida == 2) {
                        scale = blocos[i][j].bloco.width / (float)texB1.width;
                        DrawTextureEx(texB1,
                            { blocos[i][j].bloco.x, blocos[i][j].bloco.y },
                            0.0f,
                            scale,
                            WHITE
                        );
                    }
                    else if (blocos[i][j].vida == 1) {
                        scale = blocos[i][j].bloco.width / (float)texB2.width;
                        DrawTextureEx(texB2,
                            { blocos[i][j].bloco.x, blocos[i][j].bloco.y },
                            0.0f,
                            scale,
                            WHITE
                        );
                    }
                }
            }

            DrawText(TextFormat("Dificuldade: %s", textoDif), 20, 20, 20, RAYWHITE);
            DrawText(TextFormat("Fase: %d/%d", faseAtual, TOTAL_FASES), 20, 50, 20, RAYWHITE);
            DrawText(TextFormat("Vidas: %d", vidas), 20, 80, 20, RED);
            DrawText(TextFormat("Blocos quebrados: %d", blocosQuebrados), 550, 20, 20, RAYWHITE);
            DrawText(TextFormat("Tempo: %.1fs", framesJogando / 60.0f), 550, 50, 20, RAYWHITE);
            DrawText(TextFormat("Score: %d", scoreAtual), 550, 80, 20, SKYBLUE);

           for (int k = 0; k < MAX_ITENS; k++){
                if (!itens[k].ativo) continue;

                Texture2D texItem;

                switch (itens[k].tipo) {
                    case ITEM_VIDA:     texItem = texVida; break;
                    case ITEM_PAD_UP:   texItem = texPadUp; break;
                    case ITEM_PAD_DOWN: texItem = texPadDown; break;
                    case ITEM_SCORE:    texItem = texScore; break;
                }

                DrawTexture(
                    texItem,
                    itens[k].pos.x - texItem.width / 2,
                    itens[k].pos.y - texItem.height / 2,
                    WHITE
                );
            }
            DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);
            DrawTextureEx(texBola, {bolaPosicao.x - texBola.width/2, bolaPosicao.y - texBola.height/2}, 0.0f, 1.0f, WHITE);//desenha a bola


            EndDrawing();
            continue;
        }

        
        //  TELA: TRANSIÇÃO DE FASE
        
        if (telaAtual == TELA_TRANSICAO) {
            BeginDrawing();
            ClearBackground(BLACK);
            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText(TextFormat("Fase %d completa!", faseMensagem - 1),
                     screenWidth / 2 - 170, 200, 30, LIME);
            DrawText(TextFormat("Proxima fase: %d", faseMensagem),
                     screenWidth / 2 - 130, 240, 25, RAYWHITE);
            DrawText("Pressione ENTER para continuar",
                     screenWidth / 2 - 170, 300, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                telaTransicaoFase = false;
                telaAtual = TELA_JOGO;
            }

            EndDrawing();
            continue;
        }

        
        //  TELA: GAME OVER
        
        if (telaAtual == TELA_GAMEOVER) {
            BeginDrawing();
            ClearBackground(BLACK);
            for (int i = 0; i < 200; i++) DrawPixelV(estrelas[i], LIGHTGRAY);

            DrawText("GAME OVER", 250, 120, 60, RED);
            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()), 300, 200, 25, RAYWHITE);
            DrawText(TextFormat("Pontuacao final: %d", pontuacaoFinal), 250, 260, 30, LIME);
            DrawText("ENTER = Voltar ao menu", 260, 330, 20, GRAY);
            DrawText("ESC = Sair", 340, 360, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)){
                gameOver = false;
                telaAtual = TELA_MENU;
                telaDificuldade = false;
                telaNome = false;
                nomeJogador.clear();
                faseAtual = 1;
                ResetGame(blocos, true);
            }

            EndDrawing();
            continue;
            UnloadSound(somToque);
            CloseAudioDevice();
        }
        }
}

