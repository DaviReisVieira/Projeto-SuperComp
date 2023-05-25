#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <bitset>

#include <cmath>
#include <stack>
#include <omp.h>

using namespace std;

struct Filme {
    int id;
    int inicio;
    int fim;
    int categoria;
    std::bitset<24> horario;
};

struct Categoria {
    int id;
    int quantidade;
};

struct Maratona {
    std::bitset<24> disponibilidade;
    std::vector<Filme> filmes;
};

std::bitset<24> gera_horario(int inicio, int fim) {
    std::bitset<24> horario;

    if (inicio == fim) {
        horario.set(inicio);
        return horario;
    }
    
    for (int i = inicio; i < fim; i++) {
        horario.set(i);
    }

    return horario;
}

void preenche_bitset(std::bitset<24> &horarios_disponiveis, int inicio, int fim) {
    if (inicio == fim) {
        horarios_disponiveis.set(inicio);
        return;
    }

    for (int i = inicio; i < fim; i++) {
        horarios_disponiveis.set(i);
    }
}

void busca_exaustiva(vector<Filme>& filmes, vector<Categoria>& categorias, Maratona& maratona) {
    int maximo = 0;
    int size_of_filmes = filmes.size();
    cout << "Quantidade total de filmes: " << size_of_filmes << endl;

    long int todas_combinacoes = pow(2, size_of_filmes);
    cout << "Quantidade total de combinações: " << todas_combinacoes << endl;
    long int i;

    #pragma omp parallel for
    for (i = 0; i < todas_combinacoes; i++) {
        int num_films = 0;
        vector<int> categorias_vistas(categorias.size(), 0);
        stack<Filme> filmes_vistos;
        Maratona maratona_atual;

        bitset<64> filmes_vector(i);
        bitset<64> aux = filmes_vector;

        for (int j = 0; j < size_of_filmes; j++) {
            if (aux[j] == 1) {
                if ((maratona_atual.disponibilidade & filmes[j].horario) == 0) {
                    num_films++;
                    filmes_vistos.push(filmes[j]);
                    maratona_atual.disponibilidade |= filmes[j].horario;
                    categorias_vistas[filmes[j].categoria - 1]++;
                }

                aux[j] = 0;

            }
        }
        
        #pragma omp critical
        if (num_films > maximo) {
            maximo = num_films;
            maratona.filmes.clear();
            maratona.disponibilidade.reset();

            while (!filmes_vistos.empty()) {
                Filme filme = filmes_vistos.top();
                filmes_vistos.pop();
                maratona.filmes.push_back(filme);
                maratona.disponibilidade |= filme.horario;
            }
        }
    }

    

    cout << "Quantidade máxima possível: " << maximo << endl;

    for (int i = 0; i < maximo; i++) {
        cout << "Filme: [" << maratona.filmes[i].id << "] " << maratona.filmes[i].inicio << " " << maratona.filmes[i].fim << " " << maratona.filmes[i].categoria << endl;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<Filme> filmes;
    vector<Categoria> categorias(m);
    Maratona maratona;
    
    for (int i = 0; i < m; i++) {
        cin >> categorias[i].quantidade;
        categorias[i].id = i + 1;
    }

    for (int i = 0; i < n; i++) {
        int inicio, fim, categoria;
        cin >> inicio >> fim >> categoria;

        if (inicio > fim) {
            if (fim == 0){
                fim = 24;
            } else if (inicio == -1 || fim == -1) {
                continue;
            } else {
                continue;
            }
        }

        Filme filme;
        filme.id = i + 1;
        filme.inicio = inicio;
        filme.fim = fim;
        filme.categoria = categoria;
        filme.horario = gera_horario(inicio, fim);

        filmes.push_back(filme);
    }

    busca_exaustiva(filmes, categorias, maratona);

    return 0;
}
