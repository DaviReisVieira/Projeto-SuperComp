# Projeto de Super Computação - Maratona de Filmes

## Desenvolvido por

- Davi Reis Vieira de Souza

## Relatório Final

Disponível em um dos links abaixo:

[![Jupyter Notebook](https://img.shields.io/badge/jupyter-%23FA0F00.svg?style=for-the-badge&logo=jupyter&logoColor=white)](https://github.com/DaviReisVieira/Projeto-SuperComp/blob/main/RelatórioFinal.ipynb)

[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/DaviReisVieira/Projeto-SuperComp/blob/main/RelatórioFinal.ipynb)

## Descrição do Projeto

Queremos passar um final de semana assistindo ao máximo de filmes possível, mas há restrições quanto aos horários disponíveis e ao número de títulos que podem ser vistos em cada categoria (comédia, drama, ação, etc).

**Entrada**: Um inteiro N representando o número de filmes disponíveis para assistir e N trios de inteiros (H[i], F[i], C[i]), representando a hora de início, a hora de fim e a categoria do i-ésimo filme. Além disso, um inteiro M representando o número de categorias e uma lista de M inteiros representando o número máximo de filmes que podem ser assistidos em cada categoria.

Abaixo, temos o seguinte exemplo de *input*:

```txt
10 4
1 3 1 2 
11 13 3
14 15 3
10 16 2
```

Como ler esse arquivo?

- A primeira linha indica que há 10 filmes a serem considerados e 4 categorias;
- a segunda linha indica qual o máximo de filmes que cada categoria pode ter;
- da terceira linha em diante você vai encontrar os n filmes, suas respectivas hora de início, hora de término e categoria pertencente.

**Saída**: Um inteiro representando o número máximo de filmes que podem ser assistidos de acordo com as restrições de horários e número máximo por categoria, e os filmes escolhidos.

O arquivo de saída é gerado pelo programa e contém a:

- Quantidade de filmes na maratona
- Lista de filmes, com seus respectivos:
    - IDs
    - Horário de início
    - Horário de fim
    - Categoria

Exemplo de arquivo de saída:

```txt
4
474 0 1 20
99 1 2 70
184 2 4 12
581 4 5 52
```

Desta forma, é possível facilmente pegar os dados de saída e gerar um arquivo de texto com a lista de filmes da maratona e, com isso, gerar gráficos a partir dos dados, como veremos mais a frente.