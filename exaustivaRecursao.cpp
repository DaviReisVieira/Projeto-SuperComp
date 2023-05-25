#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <bitset>
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

bool compara_filme(Filme a, Filme b) {
    return a.fim < b.fim;
}

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

void heuristica_gulosa(vector<Filme> &filmes, vector<Categoria> &categorias, Maratona &maratona) {
    int maximo = 0;
    int size_of_filmes = filmes.size();

    for (int i = 0; i < size_of_filmes; i++) {
        Filme filme = filmes[i];
        int categoria = filme.categoria;
        std::bitset<24> horario = filme.horario;

        if (categorias[categoria - 1].quantidade == 0) {
            continue;
        }

        if (maratona.disponibilidade == 0) {
            maratona.disponibilidade = horario;
            maratona.filmes.push_back(filme);
            categorias[categoria - 1].quantidade--;
            maximo++;
        } else {
            if ((maratona.disponibilidade & horario) == 0) {
                maratona.disponibilidade |= horario;
                maratona.filmes.push_back(filme);
                categorias[categoria - 1].quantidade--;
                maximo++;
            }
        }
    }

    cout << maximo << endl;

    for (int i = 0; i < maximo; i++) {
        cout << maratona.filmes[i].id << " " << maratona.filmes[i].inicio << " " << maratona.filmes[i].fim << " " << maratona.filmes[i].categoria << endl;
    }
}

void exaustiva_paralela(vector<Filme> &filmes, vector<Categoria> &categorias, Maratona &maratona, int inicio, int fim, int &maximo, Maratona &melhor_maratona) {
    if (inicio == fim) {
        int count = 0;
        std::bitset<24> disponibilidade_temp = maratona.disponibilidade;

        for (int i = 0; i < inicio; i++) {
            Filme filme = filmes[i];
            int categoria = filme.categoria;
            std::bitset<24> horario = filme.horario;

            if (categorias[categoria - 1].quantidade > 0 && (disponibilidade_temp & horario) == 0) {
                disponibilidade_temp |= horario;
                count++;
            }
        }

        if (count > maximo) {
            maximo = count;
            melhor_maratona = maratona;
        }

        return;
    }

    Filme filme = filmes[inicio];
    int categoria = filme.categoria;
    std::bitset<24> horario = filme.horario;

    if (categorias[categoria - 1].quantidade > 0 && (maratona.disponibilidade & horario) == 0) {
        maratona.disponibilidade |= horario;
        maratona.filmes.push_back(filme);
        categorias[categoria - 1].quantidade--;

        exaustiva_paralela(filmes, categorias, maratona, inicio + 1, fim, maximo, melhor_maratona);

        categorias[categoria - 1].quantidade++;
        maratona.filmes.pop_back();
        maratona.disponibilidade = maratona.disponibilidade ^ horario;
    }

    exaustiva_paralela(filmes, categorias, maratona, inicio + 1, fim, maximo, melhor_maratona);
}

void exaustiva_omp(vector<Filme> &filmes, vector<Categoria> &categorias, Maratona &maratona) {
    int maximo = 0;
    Maratona melhor_maratona;

    #pragma omp parallel
    {
        Maratona maratona_local = maratona;
        int maximo_local = 0;
        #pragma omp for
        for (int i = 0; i < filmes.size(); i++) {
            Maratona maratona_temp = maratona_local;
            int maximo_temp = maximo_local;

            exaustiva_paralela(filmes, categorias, maratona_temp, i, filmes.size(), maximo_temp, melhor_maratona);

            #pragma omp critical
            {
                if (maximo_temp > maximo) {
                    maximo = maximo_temp;
                    melhor_maratona = maratona_temp;
                }
            }
        }
    }

    cout << maximo << endl;

    for (int i = 0; i < maximo; i++) {
        cout << melhor_maratona.filmes[i].id << " " << melhor_maratona.filmes[i].inicio << " " << melhor_maratona.filmes[i].fim << " " << melhor_maratona.filmes[i].categoria << endl;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<Filme> filmes(n);
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
            if (fim == 0) {
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

    sort(filmes.begin(), filmes.end(), compara_filme);

    exaustiva_omp(filmes, categorias, maratona);

    return 0;
}
