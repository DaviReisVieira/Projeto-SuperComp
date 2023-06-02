#include <iostream>
#include <vector>

#include <thrust/host_vector.h>
#include <thrust/sequence.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/copy.h>

using namespace std;

struct Filme
{
  int id;
  int inicio;
  int fim;
  int categoria;
};

void preenche_horarios(int &horarios_disponiveis, int inicio, int fim)
{
  for (int i = inicio; i < fim; i++)
  {
    horarios_disponiveis |= (1 << i);
  }
}

struct busca_exaustiva_gpu
{
  int n_filmes;
  int m_categorias;

  int *disponibilidade_categoria;
  int *horarios_filmes;
  int *categoria_filmes;

  busca_exaustiva_gpu(int n_filmes_,
                      int m_categorias_,
                      int *disponibilidade_categoria_,
                      int *horarios_filmes_,
                      int *categoria_filmes_)
      : n_filmes(n_filmes_),
        m_categorias(m_categorias_),
        disponibilidade_categoria(disponibilidade_categoria_),
        horarios_filmes(horarios_filmes_),
        categoria_filmes(categoria_filmes_)
  {
  }

  __device__ int operator()(const int &config)
  {
    int horarios_disponiveis = 0;
    int categorias_vistas[16];

    for (int i = 0; i < m_categorias; i++)
    {
      categorias_vistas[i] = disponibilidade_categoria[i];
    }

    int num_filmes = 0;
    for (int i = 0; i < n_filmes; i++)
    {
      if (config & (1 << i))
      {
        if (categorias_vistas[categoria_filmes[i] - 1] > 0)
        {
          int horario_analisado = horarios_disponiveis & horarios_filmes[i];
          if (horario_analisado != 0)
            return false;

          categorias_vistas[categoria_filmes[i] - 1]--;
          horarios_disponiveis |= horarios_filmes[i];
          num_filmes++;
        }
      }
    }

    return num_filmes;
  }
};

int main()
{
  int n, m;
  cin >> n >> m;

  vector<Filme> vetor_filmes;
  thrust::host_vector<int> disponibilidade_categoria(m);

  for (int i = 0; i < m; i++)
  {
    cin >> disponibilidade_categoria[i];
  }

  for (int i = 0; i < n; i++)
  {
    int inicio, fim, categoria;
    cin >> inicio >> fim >> categoria;

    if (inicio > fim)
    {
      if (fim == 0)
      {
        fim = 24;
      }
      else if (inicio == -1 || fim == -1)
      {
        continue;
      }
      else
      {
        continue;
      }
    }

    Filme filme;
    filme.id = i + 1;
    filme.inicio = inicio;
    filme.fim = fim;
    filme.categoria = categoria;

    vetor_filmes.push_back(filme);
  }

  int n_filmes = vetor_filmes.size();

  thrust::host_vector<int> categoria_filmes(n_filmes);
  thrust::host_vector<int> horarios_filmes_cpu(n_filmes);

  for (int i = 0; i < n_filmes; i++)
  {
    horarios_filmes_cpu[i] = 0;
    preenche_horarios(horarios_filmes_cpu[i],
                      vetor_filmes[i].inicio,
                      vetor_filmes[i].fim);
    categoria_filmes[i] = vetor_filmes[i].categoria;
  }

  thrust::device_vector<int> vetor_possibilidades_gpu(pow(2, n_filmes));

  thrust::sequence(vetor_possibilidades_gpu.begin(), vetor_possibilidades_gpu.end());

  thrust::device_vector<int> disponibilidade_categoria_gpu(disponibilidade_categoria);

  thrust::device_vector<int> horarios_filmes_gpu(horarios_filmes_cpu);
  thrust::device_vector<int> categoria_filmes_gpu(categoria_filmes);

  thrust::transform(
      vetor_possibilidades_gpu.begin(),
      vetor_possibilidades_gpu.end(),
      vetor_possibilidades_gpu.begin(),
      busca_exaustiva_gpu(n_filmes,
                          m,
                          raw_pointer_cast(disponibilidade_categoria_gpu.data()),
                          raw_pointer_cast(horarios_filmes_gpu.data()),
                          raw_pointer_cast(categoria_filmes_gpu.data())));

  thrust::host_vector<int> config_vector_cpu_final = vetor_possibilidades_gpu;

  int max_count = *thrust::max_element(config_vector_cpu_final.begin(), config_vector_cpu_final.end());

  cout << max_count << endl;

  int max_config = -1;
  for (int i = 0; i < config_vector_cpu_final.size(); i++)
  {
    if (config_vector_cpu_final[i] == max_count)
    {
      max_config = i;
      break;
    }
  }

  for (int i = 0; i < n_filmes; i++)
  {
    if (max_config & (1 << i))
    {
      cout << vetor_filmes[i].id << " " << vetor_filmes[i].inicio << " " << vetor_filmes[i].fim << " " << vetor_filmes[i].categoria << endl;
    }
  }

  return 0;
}