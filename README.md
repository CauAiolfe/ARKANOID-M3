# ARKANOID-M3

Este projeto implementa uma versão do jogo Arkanoid utilizando a linguagem C++ e a biblioteca Raylib. O jogo inclui sistema de fases, itens especiais, ranking, dificuldades e telas completas de menu.

Objetivos do Projeto.

Desenvolver um jogo funcional aplicando conceitos fundamentais de programação em C++.
Utilizar a biblioteca Raylib para renderização gráfica, entrada do usuário, manipulação de texturas e reprodução de áudio.
Implementar mecânicas de colisão, pontuação, controle de vidas e progressão de fases.
Organizar o código em múltiplos módulos (arquivos .cpp e .h).
Criar um sistema de ranking persistente utilizando leitura e gravação de arquivos.

Bibliotecas Utilizadas
Raylib

Utilizada para:
Criar a janela e controlar o loop principal.
Desenhar texturas e formas geométricas.
Carregar e reproduzir sons.
Capturar entradas via teclado e mouse.

C++ STL
Bibliotecas padrão utilizadas:
<iostream>
<cmath>
<string>
<vector>
<fstream>
Usadas para lógica geral do jogo, manipulação de strings, cálculos, armazenamento de dados e acesso ao ranking.

Controles do Jogo

Seta Esquerda – Move a plataforma para a esquerda
Seta Direita – Move a plataforma para a direita
Mouse – Navegação no menu
Clique Esquerdo – Seleciona opções
ENTER – Confirma nome, avança telas
ESC – Volta ao menu anterior (em telas adequadas)

O objetivo é destruir todos os blocos da fase utilizando a bola. A bola rebate nas paredes, na plataforma e nos blocos. Cada bloco pode ter múltiplas vidas e muda de aparência conforme perde vida. Quando os blocos são destruídos, o jogador marca pontos. Itens especiais podem cair de blocos destruídos e gerar efeitos como vida extra, aumento da plataforma, redução da plataforma e pontos extras. O jogador inicia com 3 vidas; perde uma vida quando a bola ultrapassa a parte inferior da tela; com 0 vidas, o jogo vai para a tela de Game Over. Há três dificuldades: Fácil (plataforma maior e bola mais lenta), Médio (configuração padrão) e Difícil (plataforma menor e bola mais rápida), que ajustam a jogabilidade conforme a escolha. O jogo possui múltiplas fases, e ao destruir todos os blocos o jogador avança para a próxima. Ao finalizar o jogo, o nome e a pontuação total são salvos em arquivo, e o ranking exibe os 5 melhores resultados.

Estrutura do Projeto
main.cpp
fases.cpp / fases.h
itens.cpp / itens.h
ranking.cpp / ranking.h
assents/
    bloco.png
    blocoB1.png
    blocoB2.png
    bola.png
    vidaextra.png
    aumentaBase.png
    diminuiBase.png
    pontuacao.png
    somBatida.wav
    
Compilação:

g++ -o main.exe main.cpp fase.cpp itens.cpp ranking.cpp -I include -L lib -lraylib -lgdi32 -lwinmm

./main.exe





