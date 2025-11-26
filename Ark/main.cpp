#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>

#include "fases.h"
#include "itens.h"
#include "ranking.h"

using namespace std;

//fases
int faseAtual = 1;
int faseMensagem = 1;
const int TOTAL_FASES = 3;
bool telaTransicaoFase = false;

//plataforma 
Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim = {450.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};
Color cor = MAROON;

//bola
Vector2 bolaPosicao = {400.0f, 539.0f};//posicao inicial da bola na tela 

//movimentacao da bola
int direcao = -1;
int diagonal = 1;
float bolaVelocidadeY = SPEEDBOLA;
float bolaVelocidadeX = SPEEDBOLA;
bool moveDiagonal = false;

//estados do jogo
bool gameOver = false;
bool telaNome = false;
bool telaDificuldade = true;

//pontuacao e vidas
int blocosQuebrados = 0;
int framesJogando = 0;
int pontuacaoFinal = 0;
int scoreAtual = 0;
int vidas = 3;

//nome do jogador
string nomeJogador = "";

//dificuldade atual
Dificuldade dificuldadeAtual = FACIL;//dificuldade inicial padrao (facil)

//reinicia o jogo (posicoes, blocos). se resettotais = true, zera pontuacoes e volta pra fase 1
void ResetGame(Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS], bool resetTotais){//recebe a matriz de blocos e uma flag para resetar tudo ou nao
    if (resetTotais){//se for um novo jogo (e nao apenas troca de fase)
        blocosQuebrados = 0;
        framesJogando = 0;
        pontuacaoFinal = 0;
        scoreAtual = 0;
        vidas = 3;
        faseAtual = 1;
        faseMensagem = 1;
    }

    GameConfig cfg = GetConfig(dificuldadeAtual);//chama getconfig com a dificuldade atual para obter as configuracoes do jogo

    //tamanho da plataforma conforme dificuldade
    plataformaTamanho = {cfg.larguraPlataforma, 20.0f};//ajusta a largura da plataforma de acordo com a dificuldade 

    plataformaPosicao = {350.0f, 550.0f};//reposiciona a plataforma no ponto inicial (x=350, y=550)
    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};//recalcula o ponto final (x final = x inicial + largura)

    bolaPosicao = {400.0f, 539.0f};//reposiciona a bola na posicao inicial 
    direcao = -1;//bola comeca indo para cima
    diagonal = 1;
    bolaVelocidadeY = cfg.velocidadeBola;//velocidade vertical da bola baseada na dificuldade
    bolaVelocidadeX = cfg.velocidadeBola;//velocidade horizontal da bola baseada na dificuldade
    moveDiagonal = false;//se true vai na diagonal 

    ClearItems();
    ConfigurarFase(blocos, faseAtual, cfg);//chama funcao do modulo fases para montar a fase 
}

int main(){
    const int screenWidth = 800;//largura da janela do jogo
    const int screenHeight = 600;//altura da janela do jogo

    InitWindow(screenWidth, screenHeight, "Arkanoid");//inicializa a janela
    SetTargetFPS(60);

    //fundo de estrelas cria 200 posicoes para estrelas
    static Vector2 estrelas[200];
    static bool estrelasInit = false;//se as estrelas ja foram inicializadas

    //nao deixar esc fechar automaticamente
    SetExitKey(0);

    Blocos blocos[QUANTIDADELINHASBLOCOS][QUANTIDADEBLOCOS];//declara a matriz de blocos do jogo
    ResetGame(blocos, true);//chama resetgame para configurar o jogo pela primeira vez

    while (true){//loop principal do jogo

        if (WindowShouldClose()) break;
        if (IsKeyPressed(KEY_ESCAPE)) break;

        //inicializa as estrelas uma vez
        if (!estrelasInit){
            for (int i = 0; i < 200; i++){//laco para preencher todas as 200 estrelas
                estrelas[i] = {
                    (float)GetRandomValue(0, screenWidth),//posicao x aleatoria entre 0 e largura da tela
                    (float)GetRandomValue(0, screenHeight)//posicao y aleatoria entre 0 e altura da tela
                };
            }
            estrelasInit = true;//marca que as estrelas ja foram inicializadas
        }

        //tela de dificuldade
        if (telaDificuldade){//se estamos na tela de selecao de dificuldade

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){//se apertar seta para cima ou tecla w
                dificuldadeAtual = (Dificuldade)((((int)dificuldadeAtual) + 2) % 3);//muda a dificuldade para a opcao de cima
            } // ciclo 0,1,2
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){//se apertar seta para baixo ou tecla s
                dificuldadeAtual = (Dificuldade)((((int)dificuldadeAtual) + 1) % 3);//muda a dificuldade para a opcao de baixo 
            }

            if (IsKeyPressed(KEY_ENTER)){//se apertar enter
                telaDificuldade = false;//sai da tela de dificuldade
                telaNome = true;//entra na tela de digitar o nome
                ResetGame(blocos, true);//reinicia o jogo como novo 
            }

            BeginDrawing();//desenho
            ClearBackground(BLACK);//limpa o fundo da tela com a cor preta

            //fundo de estrelas na tela de dificuldade tambem
            for (int i = 0; i < 200; i++){
                estrelas[i].y += 1.5f;//desloca a estrela para baixo (efeito de movimento)
                if (estrelas[i].y > screenHeight){//se ela passou da parte de baixo da tela
                    estrelas[i].y = 0;//volta para o topo
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);//sorteia uma nova posicao x
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);//desenha a estrela como um pixel na cor cinza clara
            }

            DrawText("SELECIONE A DIFICULDADE", screenWidth / 2 - 210, 120, 30, RAYWHITE);//texto titulo da tela de dificuldade
            DrawText("Use SETAS CIMA/BAIXO ou W/S para trocar", screenWidth / 2 - 260, 170, 20, GRAY);//instrucao de como trocar
            DrawText("ENTER para confirmar", screenWidth / 2 - 130, 200, 20, GRAY);//instrucao de como confirmar

            const char* textoFacil = "FACIL";//texto da dificuldade facil
            const char* textoMedio = "MEDIO";
            const char* textoDificil = "DIFICIL";

            Color corFacil = (dificuldadeAtual == FACIL) ? RED : DARKGRAY;//se a dificuldade atual e facil, pinta em vermelho, senao cinza
            Color corMedio = (dificuldadeAtual == MEDIO) ? RED : DARKGRAY;
            Color corDificil = (dificuldadeAtual == DIFICIL) ? RED : DARKGRAY;

            DrawText(textoFacil, screenWidth / 2 - 50, 260, 30, corFacil);//desenha a palavra facil na tela
            DrawText(textoMedio, screenWidth / 2 - 50, 310, 30, corMedio);
            DrawText(textoDificil, screenWidth / 2 - 60, 360, 30, corDificil);

            //top 5 ranking
            vector<Score> top5 = LerRankingTop5();//chama a funcao de ranking para pegar o vetor com os 5 melhores scores
            DrawText("TOP 5 RANKING:", 40, 410, 20, YELLOW);//desenha o titulo da area de ranking

            for (int i = 0; i < (int)top5.size(); i++){//percorre cada elemento do vetor de scores
                DrawText(
                    TextFormat("%d. %s - %d",
                               i + 1,//posicao no ranking (1, 2, 3, 4, 5)
                               top5[i].nome.c_str(),//nome do jogador convertido para const char*
                               top5[i].pontos),//pontos do jogador
                    40,
                    440 + i * 25,//desloca cada linha de texto 25 pixels para baixo
                    18,
                    RAYWHITE
                );
            }

            EndDrawing();
            continue;
        }

        //tela de nome
        if (telaNome){//se estamos na tela de digitacao do nome
            int key = GetKeyPressed();
            while (key > 0){//laco para processar varias teclas na mesma iteracao do frame
                if (key == KEY_BACKSPACE && !nomeJogador.empty()){//se a tecla for backspace e o nome nao estiver vazio
                    nomeJogador.pop_back();//remove o ultimo caractere do nome
                }
                else if (key >= 32 && key <= 125){//se o codigo da tecla esta no intervalo de caracteres imprimiveis
                    nomeJogador.push_back((char)key);//converte o codigo da tecla para char e adiciona ao nome
                }
                key = GetKeyPressed();//pega a proxima tecla (se houver)
            }

            if (IsKeyPressed(KEY_ENTER) && !nomeJogador.empty()){//se apertar enter e o nome nao estiver vazio
                telaNome = false;//sai da tela de nome
            }

            BeginDrawing();
            ClearBackground(BLACK);

            //fundo de estrelas aqui tambem
            for (int i = 0; i < 200; i++){
                estrelas[i].y += 1.5f;
                if (estrelas[i].y > screenHeight){
                    estrelas[i].y = 0;
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);
            }

            DrawText("Digite seu nome:", screenWidth / 2 - 150, 200, 30, RAYWHITE);//titulo pedindo o nome

            DrawRectangle(screenWidth / 2 - 150, 250, 300, 50, DARKGRAY);//retangulo de fundo da caixa de texto
            DrawText(nomeJogador.c_str(), screenWidth / 2 - 140, 260, 30, SKYBLUE);//mostra o nome digitado

            DrawText("Pressione ENTER para continuar",
                     screenWidth / 2 - 160, 330, 20, GRAY);//instrucao de como avancar

            EndDrawing();
            continue;
        }

        //tela de transicao de fase
        if (telaTransicaoFase){//se estamos na tela de "fase completa"
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 200; i++){//atualiza e desenha estrelas no fundo
                estrelas[i].y += 1.5f;
                if (estrelas[i].y > screenHeight){
                    estrelas[i].y = 0;
                    estrelas[i].x = (float)GetRandomValue(0, screenWidth);
                }
                DrawPixelV(estrelas[i], LIGHTGRAY);
            }

            DrawText(TextFormat("Fase %d completa!", faseMensagem - 1),
                     screenWidth / 2 - 170, 200, 30, LIME);//mensagem de fase anterior concluida
            DrawText(TextFormat("Proxima fase: %d", faseMensagem),
                     screenWidth / 2 - 130, 240, 25, RAYWHITE);//mensagem com o numero da proxima fase
            DrawText("Pressione ENTER para continuar",
                     screenWidth / 2 - 170, 300, 20, GRAY);//instrucao para continuar

            if (IsKeyPressed(KEY_ENTER)){
                telaTransicaoFase = false;//sai da tela de transicao de fase
            }

            EndDrawing();
            continue;
        }

        //logica do jogo
        if (!gameOver && !telaTransicaoFase){//se nao esta em game over e nao esta na tela de transicao, roda a logica normal de jogo

            framesJogando++;//incrementa a contagem de frames (usado como base de tempo)

            if (IsKeyDown(KEY_RIGHT) && plataformaPosicao.x < screenWidth - (plataformaTamanho.x + 10)){
                //se seta direita esta pressionada e a plataforma ainda nao chegou na borda da direita
                plataformaPosicao.x += 10.0f;//move a plataforma para a direita
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;//atualiza a posicao final
            }
            if (IsKeyDown(KEY_LEFT) && plataformaPosicao.x > 10){
                //se seta esquerda esta pressionada e a plataforma ainda nao encostou na borda esquerda
                plataformaPosicao.x -= 10.0f;//move a plataforma para a esquerda
                plataformaPosicaoFim.x = plataformaPosicao.x + plataformaTamanho.x;//atualiza a posicao final
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         plataformaPosicao, plataformaPosicaoFim)){
                //verifica colisao entre a bola e a linha representando a parte de cima da plataforma
                direcao *= -1;//inverte a direcao vertical (faz a bola quicar)

                if (bolaPosicao.x - plataformaPosicao.x < plataformaTamanho.x / 2.0f){
                    //se a bola bateu na metade esquerda da plataforma
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((plataformaTamanho.x / 2.0f - (bolaPosicao.x - plataformaPosicao.x)) /
                                       (plataformaTamanho.x / 2.0f));//calcula velocidade x proporcional a distancia do centro
                    moveDiagonal = true;//ativa movimento diagonal
                    diagonal = -1;//manda a bola para a esquerda
                }
                else if (bolaPosicao.x - plataformaPosicao.x > plataformaTamanho.x / 2.0f){
                    //se a bola bateu na metade direita da plataforma
                    bolaVelocidadeX = bolaVelocidadeY *
                                      ((bolaPosicao.x - plataformaPosicao.x - plataformaTamanho.x / 2.0f) /
                                       (plataformaTamanho.x / 2.0f));//calcula velocidade x proporcional
                    moveDiagonal = true;//ativa diagonal
                    diagonal = 1;//manda a bola para a direita
                }
            }

            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {0.0f, 10.0f}, {800.0f, 10.0f}))
                //colisao com a borda de cima (linha y = 10)
                direcao *= -1;//inverte direcao vertical (bola quica no teto)
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {10.0f, 0.0f}, {10.0f, 600.0f}))
                //colisao com a borda esquerda (x = 10)
                diagonal = 1;//forca a bola a ir para a direita
            if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA,
                                         {790.0f, 0.0f}, {790.0f, 600.0f}))
                //colisao com a borda direita (x = 790)
                diagonal = -1;//forca a bola a ir para a esquerda

            bolaPosicao.y += bolaVelocidadeY * direcao;//atualiza a posicao vertical da bola (subindo ou descendo)
            if (moveDiagonal)
                bolaPosicao.x += bolaVelocidadeX * diagonal;//se o movimento diagonal estiver ativo, atualiza a posicao horizontal

            //se a bola cair fora da tela -> perde vida
            if (bolaPosicao.y - TAMANHOBOLA > screenHeight){
                //se o topo da bola passou da parte inferior da tela (y > screenheight)
                vidas--;//perde uma vida
                if (vidas <= 0){//se nao tiver mais vidas
                    gameOver = true;//ativa estado de game over
                    int pontosTempo = framesJogando;//usa o numero de frames como base de tempo (pontos de tempo)
                    int pontosBlocos = scoreAtual;//usa o score atual como pontos de blocos (ja inclui itens)
                    pontuacaoFinal = pontosTempo + pontosBlocos;//soma os dois para gerar pontuacao final
                    SalvarRanking(nomeJogador, pontuacaoFinal);//salva no ranking (arquivo txt)
                } else {
                    //se ainda restam vidas, apenas reseta posicao da bola e plataforma
                    plataformaPosicao = {350.0f, 550.0f};//volta a plataforma para o meio
                    plataformaPosicaoFim = {plataformaPosicao.x + plataformaTamanho.x, plataformaPosicao.y};//recalcula x final
                    bolaPosicao = {400.0f, 539.0f};//reposiciona a bola
                    direcao = -1;//bola volta indo pra cima
                    diagonal = 1;//diagonal para a direita
                    moveDiagonal = false;//inicialmente sem movimento diagonal
                }
            }

            Vector2 futuraX = {bolaPosicao.x + (bolaVelocidadeX * diagonal), bolaPosicao.y};
            //futura posicao da bola so no eixo x (usada para detectar colisao lateral com blocos)
            Vector2 futuraY = {bolaPosicao.x, bolaPosicao.y + (bolaVelocidadeY * direcao)};
            //futura posicao da bola so no eixo y (usada para detectar colisao vertical com blocos)

            bool houveColisao = false;//flag para saber se ja houve colisao com algum bloco nesse frame

            for (int i = 0; i < QUANTIDADELINHASBLOCOS && !houveColisao; i++){//percorre linhas de blocos enquanto nao houve colisao
                for (int j = 0; j < QUANTIDADEBLOCOS && !houveColisao; j++){//percorre colunas de blocos enquanto nao houve colisao

                    if (!blocos[i][j].ativo) continue;//se o bloco nao esta ativo (ja foi destruido), pula

                    Rectangle r = blocos[i][j].bloco;//pega o retangulo do bloco atual

                    bool colisaoX = CheckCollisionCircleRec(futuraX, TAMANHOBOLA, r);//verifica colisao no cenario de movimento horizontal
                    bool colisaoY = CheckCollisionCircleRec(futuraY, TAMANHOBOLA, r);//verifica colisao no cenario de movimento vertical

                    if (colisaoX || colisaoY){//se houve colisao em x ou em y
                        if (colisaoX && !colisaoY){//se so houve colisao no eixo x
                            diagonal = -diagonal;//inverte o lado da diagonal (rebate horizontalmente)
                        }
                        else if (colisaoY && !colisaoX){//se so houve colisao no eixo y
                            direcao = -direcao;//inverte a direcao vertical (rebate verticalmente)
                        }
                        else {//se houve colisao nas duas projecoes (mais central)
                            if (fabs(bolaPosicao.x - (r.x + r.width / 2)) >
                                fabs(bolaPosicao.y - (r.y + r.height / 2)))
                                //se a diferenca no eixo x for maior que no eixo y, considera colisao lateral
                                diagonal = -diagonal;//inverte horizontalmente
                            else
                                direcao = -direcao;//senao, inverte verticalmente
                        }

                        blocos[i][j].vida--;//diminui a vida do bloco acertado
                        if (blocos[i][j].vida <= 0){//se a vida chegou a zero ou menor
                            blocos[i][j].ativo = false;//desativa o bloco
                            blocos[i][j].bloco.x = -200;//joga o bloco para fora da tela (pra garantir que nao colida mais)
                            blocos[i][j].bloco.y = -200;
                            blocosQuebrados++;//incrementa contador de blocos destruidos
                            scoreAtual += 50;//adiciona 50 pontos ao score por destruir esse bloco
                            SpawnItem(r);//tenta gerar um item especial na posicao desse bloco (modulo itens)
                        } else {
                            //se ainda restam vidas no bloco, ele muda de cor para indicar dano
                            if (blocos[i][j].vida == 2)
                                blocos[i][j].cor = ORANGE;//quando tiver 2 de vida, fica laranja
                            else if (blocos[i][j].vida == 1)
                                blocos[i][j].cor = YELLOW;//quando tiver 1 de vida, fica amarelo
                        }

                        houveColisao = true;//marca que houve colisao e sai dos loops
                    }
                }
            }

            //atualiza itens especiais (queda + coleta)
            Rectangle paddleRect = {
                plataformaPosicao.x,
                plataformaPosicao.y,
                plataformaTamanho.x,
                plataformaTamanho.y
            };//cria um retangulo representando a plataforma (para testar colisao com itens)

            AtualizarItens((float)screenHeight,//altura da tela (limite inferior)
                           paddleRect,//retangulo do paddle
                           vidas,//referencia para as vidas (pode ser alterada pelos itens)
                           plataformaPosicao,//referencia para posicao da plataforma
                           plataformaTamanho,//referencia para tamanho da plataforma
                           plataformaPosicaoFim,//referencia para posicao final da plataforma
                           screenWidth,//largura da tela (limite horizontal)
                           scoreAtual);//referencia para score atual (pode ser alterado por itens)

            //verifica se acabou a fase
            if (!gameOver){//so verifica se ainda nao esta em game over
                int ativos = 0;//contador de blocos ativos
                for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++){
                    for (int j = 0; j < QUANTIDADEBLOCOS; j++){
                        if (blocos[i][j].ativo) ativos++;//conta blocos ainda ativos
                    }
                }

                if (ativos == 0){//se nao sobrou nenhum bloco ativo
                    if (faseAtual < TOTAL_FASES){//se ainda nao chegou na ultima fase
                        faseAtual++;//vai para a proxima fase
                        faseMensagem = faseAtual;//atualiza a fase que sera mostrada na tela de transicao
                        ResetGame(blocos, false);//reseta a fase atual (sem zerar score, vidas, etc)
                        telaTransicaoFase = true;//ativa a tela de transicao de fase
                    } else {
                        //se terminou a ultima fase
                        gameOver = true;//entra em game over
                        int pontosTempo = framesJogando;//pontos de tempo
                        int pontosBlocos = scoreAtual;//pontos de blocos + itens
                        pontuacaoFinal = pontosTempo + pontosBlocos;//soma tudo
                        SalvarRanking(nomeJogador, pontuacaoFinal);//salva no ranking
                    }
                }
            }
        }
        else if (gameOver){//se nao esta na logica normal e esta em game over
            if (IsKeyPressed(KEY_ENTER)){//se apertar enter na tela de game over
                gameOver = false;//sai do estado de game over
                telaDificuldade = true;//volta para a tela de dificuldade (menu inicial)
                telaNome = false;//garante que tela de nome esteja desativada
                nomeJogador.clear();//limpa o nome do jogador para digitar de novo
                ResetGame(blocos, true);//reinicia o jogo com reset completo
            }
        }

        //desenho
        BeginDrawing();//inicia o desenho do frame
        ClearBackground(BLACK);//limpa a tela com cor preta

        for (int i = 0; i < 200; i++){//atualiza e desenha as estrelas no fundo
            estrelas[i].y += 1.5f;
            if (estrelas[i].y > screenHeight){
                estrelas[i].y = 0;
                estrelas[i].x = (float)GetRandomValue(0, screenWidth);
            }
            DrawPixelV(estrelas[i], LIGHTGRAY);
        }

        if (!gameOver){//se nao esta em game over, desenha a tela de jogo
            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()),
                     20, 20, 25, RAYWHITE);//desenha o nome do jogador no topo

            const char* textoDif;//ponteiro para armazenar o texto da dificuldade
            switch (dificuldadeAtual){//escolhe o texto baseado na dificuldade
                case FACIL: textoDif = "FACIL"; break;
                case MEDIO: textoDif = "MEDIO"; break;
                case DIFICIL: textoDif = "DIFICIL"; break;
            }
            DrawText(TextFormat("Dificuldade: %s", textoDif), 20, 50, 20, GRAY);//desenha qual e a dificuldade atual
            DrawText(TextFormat("Fase: %d/%d", faseAtual, TOTAL_FASES),
                     20, 80, 20, GRAY);//desenha o numero da fase atual

            DrawText(TextFormat("Vidas: %d", vidas),
                     20, 110, 20, RED);//desenha quantas vidas restam

            DrawText(TextFormat("Blocos quebrados: %d", blocosQuebrados),
                     550, 20, 20, RAYWHITE);//desenha o numero de blocos destruidos
            DrawText(TextFormat("Tempo: %.1fs", framesJogando / 60.0f),
                     550, 50, 20, RAYWHITE);//desenha o tempo de jogo em segundos (frames / 60)

            DrawText(TextFormat("Score: %d", scoreAtual),
                     550, 80, 20, SKYBLUE);//desenha o score atual

            for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++){//percorre todas as linhas de blocos
                for (int j = 0; j < QUANTIDADEBLOCOS; j++){//percorre todas as colunas de blocos
                    if (!blocos[i][j].ativo) continue;//se o bloco nao esta ativo, pula

                    DrawRectangle(blocos[i][j].bloco.x,//desenha o bloco na cor e posicao definidas
                                  blocos[i][j].bloco.y,
                                  blocos[i][j].bloco.width,
                                  blocos[i][j].bloco.height,
                                  blocos[i][j].cor);
                }
            }

            //desenha itens especiais
            for (int k = 0; k < MAX_ITENS; k++){//percorre o array global de itens
                if (!itens[k].ativo) continue;//se o item nao estiver ativo, pula
                DrawCircleV(itens[k].pos, itens[k].radius, itens[k].cor);//desenha o item como um circulo colorido
            }

            DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);//desenha a plataforma (paddle)
            DrawCircleV(bolaPosicao, TAMANHOBOLA, GREEN);//desenha a bola
        }
        else {//se esta em game over
            DrawText("GAME OVER", 250, 120, 60, RED);//desenha mensagem de game over

            DrawText(TextFormat("Jogador: %s", nomeJogador.c_str()),
                     300, 200, 25, RAYWHITE);//mostra o nome do jogador

            DrawText(TextFormat("Pontuacao final: %d", pontuacaoFinal),
                     250, 260, 30, LIME);//mostra a pontuacao final

            DrawText("ENTER = Voltar ao menu", 260, 330, 20, GRAY);//instrucao para voltar ao menu
            DrawText("ESC = Sair", 340, 360, 20, GRAY);//instrucao para sair do jogo
        }

        EndDrawing();//finaliza o desenho do frame
    }

    CloseWindow();//fecha a janela e libera os recursos do raylib
    return 0;//retorna 0 indicando que o programa terminou sem erros
}
