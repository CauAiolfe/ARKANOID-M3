#ifndef RANKING_H
#define RANKING_H

#include <string>
#include <vector>
using namespace std;
struct Score {
    string nome;
    int pontos;
};

vector<Score> LerRankingTop5();
void SalvarRanking(const string& nome, int pontos);
#endif