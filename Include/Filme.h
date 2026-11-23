#ifndef FILME_H
#define FILME_H

// Estrutura que guarda as informações de um filme.
typedef struct {
    int id;                     // Código que identifica o filme
    char titulo[101];           // Nome do filme
    int duracao;                // Duração em minutos
    char sinopse[501];          // Resumo do filme
    char classificacao[21];     // Classificação indicativa
    int anoLanc;                // Ano de lançamento
    char situacao[31];          // Situação do filme no cinema
} Filme;


// Cria o espaço na memória para guardar os filmes.
// Recebe a capacidade inicial e retorna o endereço da estrutura.
// Retorna NULL se não conseguir reservar a memória.
Filme* criarFilmes(int capacidade);


// Adiciona um novo filme ao cadastro.
// Recebe a lista de filmes, a quantidade atual, a capacidade e o novo filme.
// Retorna 1 se conseguir adicionar ou 0 se ocorrer algum erro.
int inserirFilme(Filme** filmes, int* quantidade,
                 int* capacidade, Filme novo);


// Procura um filme pelo seu código (ID).
// Recebe a lista de filmes, a quantidade e o ID procurado.
// Retorna a posição do filme na lista ou -1 se não encontrar.
int buscarFilmePorId(Filme* filmes, int quantidade, int id);


// Remove um filme do cadastro usando seu ID.
// Recebe a lista de filmes, a quantidade atual e o ID.
// Retorna 1 se remover ou 0 se o filme não for encontrado.
int removerFilme(Filme* filmes, int* quantidade, int id);


// Altera as informações de um filme já cadastrado.
// Recebe a lista, a quantidade, o ID e os novos dados do filme.
// Retorna 1 se atualizar ou 0 se não encontrar o filme.
int atualizarFilme(Filme* filmes, int quantidade,
                   int id, Filme novosDados);


// Mostra todos os filmes que estão cadastrados.
// Recebe a lista de filmes e a quantidade de registros.
// Não retorna nenhum valor.
void listarFilmes(Filme* filmes, int quantidade);


// Libera o espaço da memória que foi utilizado pela lista de filmes.
// Recebe a lista de filmes.
// Não retorna nenhum valor.
void liberarFilmes(Filme* filmes);

#endif
