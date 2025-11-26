#include "ranking.h"
#include <fstream>
#include <algorithm>

using namespace std;

// nome fixo do arquivo de ranking
static const char* ARQUIVO_RANKING = "ranking.txt";

// lê o arquivo de ranking e devolve um vetor com, no máximo, top 5
vector<Score> LerRankingTop5() {
    vector<Score> scores;

    ifstream arq(ARQUIVO_RANKING);
    if (!arq.is_open()) {
        // se o arquivo ainda não existe, só retorna vazio
        return scores;
    }

    string linha;
    while (getline(arq, linha)) {
        // ignora linhas vazias
        if (linha.empty()) continue;

        // procura o '-' que separa nome e pontos
        size_t pos = linha.rfind('-');
        if (pos == string::npos) continue; // linha fora do formato, ignora

        // separa nome e pontos 
        string nome      = linha.substr(0, pos);      
        string pontosStr = linha.substr(pos + 1);    
        // remove espaços no começo 
        while (!pontosStr.empty() && (pontosStr[0] == ' ' || pontosStr[0] == '\t'))
            pontosStr.erase(0, 1);

        // se depois disso ainda tiver algo, tenta converter para int
        if (!pontosStr.empty()) {
            int pts = stoi(pontosStr); // aqui assumimos que é número válido
            scores.push_back({ nome, pts });
        }
    }

    arq.close();

    // ordena do maior para o menor
    sort(scores.begin(), scores.end(),
         [](const Score& a, const Score& b) {
             return a.pontos > b.pontos;
         });

    // deixa só os 5 primeiros, se tiver mais que 5
    if (scores.size() > 5) {
        scores.resize(5);
    }

    return scores;
}

// salva uma nova linha no ranking
void SalvarRanking(const string& nome, int pontos) {
    ofstream arq(ARQUIVO_RANKING, ios::app); // append = acrescenta no final

    if (arq.is_open()) {
        arq << nome << " - " << pontos << "\n";
        arq.close();
    }
}