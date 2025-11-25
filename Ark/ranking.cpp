#include "ranking.h"
#include <fstream>
#include <algorithm>

using namespace std;

string Trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

vector<Score> LerRankingTop5(const string& arquivo) {
    vector<Score> scores;
    ifstream arq(arquivo);
    if (!arq.is_open()) return scores;

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue;
        size_t pos = linha.rfind('-');
        if (pos == string::npos) continue;

        string nome      = Trim(linha.substr(0, pos));
        string pontosStr = Trim(linha.substr(pos + 1));

        try {
            int pts = stoi(pontosStr);
            scores.push_back({ nome, pts });
        } catch (...) {
            // linha zoada, ignora
        }
    }
    arq.close();

    sort(scores.begin(), scores.end(),
         [](const Score& a, const Score& b) {
             return a.pontos > b.pontos; // maior primeiro
         });

    if (scores.size() > 5) scores.resize(5);
    return scores;
}

void SalvarRanking(const string& nome, int pontos, const string& arquivo) {
    ofstream arq(arquivo, ios::app); // append
    if (arq.is_open()) {
        arq << nome << " - " << pontos << "\n";
        arq.close();
    }
}