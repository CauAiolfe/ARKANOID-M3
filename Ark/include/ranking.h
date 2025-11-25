#ifndef RANKING_H
#define RANKING_H

#include <string>
#include <vector>

struct Score {
    std::string nome;
    int pontos;
};

// remove espaços em branco no início/fim da string
std::string Trim(const std::string& s);

// lê o arquivo de ranking e retorna o TOP 5
std::vector<Score> LerRankingTop5(const std::string& arquivo = "ranking.txt");

// salva uma nova linha de ranking no arquivo
void SalvarRanking(const std::string& nome, int pontos, const std::string& arquivo = "ranking.txt");

#endif