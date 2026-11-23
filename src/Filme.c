#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#include "filme.h"

/* Verifica se os dados de um filme são válidos. */
static int filmeValido(Filme filme) {

    if (filme.id <= 0 || filme.duracao <= 0 || filme.anoLanc <= 0) {
        return 0;
    }

    /* Verifica se os textos possuem final de string. */
    if (memchr(filme.titulo, '\0', sizeof(filme.titulo)) == NULL ||
        memchr(filme.sinopse, '\0', sizeof(filme.sinopse)) == NULL ||
        memchr(filme.classificacao, '\0', sizeof(filme.classificacao)) == NULL ||
        memchr(filme.situacao, '\0', sizeof(filme.situacao)) == NULL) {
        return 0;
    }

    /* O título não pode estar vazio. */
    if (filme.titulo[0] == '\0') {
        return 0;
    }

    return 1;
}

/* Cria o espaço na memória para armazenar os filmes. */
Filme* criarFilmes(int capacidade) {

    if (capacidade <= 0 ||
        (size_t)capacidade > SIZE_MAX / sizeof(Filme)) {
        return NULL;
    }

    Filme* filmes = malloc(capacidade * sizeof(Filme));

    return filmes;
}

/* Insere um novo filme no cadastro. */
int inserirFilme(Filme** filmes, int* quantidade,
                 int* capacidade, Filme novo) {

    if (filmes == NULL || quantidade == NULL ||
        capacidade == NULL || !filmeValido(novo)) {
        return 0;
    }

    if (*quantidade < 0 || *capacidade < 0 ||
        *quantidade > *capacidade) {
        return 0;
    }

    if (*capacidade > 0 && *filmes == NULL) {
        return 0;
    }

    /* Impede o cadastro de dois filmes com o mesmo ID. */
    if (buscarFilmePorId(*filmes, *quantidade, novo.id) != -1) {
        return 0;
    }

    /* Verifica se o vetor precisa aumentar de tamanho. */
    if (*quantidade == *capacidade) {

        int novaCapacidade;

        if (*capacidade == 0) {
            novaCapacidade = 1;
        } else {
            if (*capacidade > INT_MAX / 2) {
                return 0;
            }

            novaCapacidade = *capacidade * 2;
        }

        if ((size_t)novaCapacidade > SIZE_MAX / sizeof(Filme)) {
            return 0;
        }

        Filme* temporario = realloc(
            *filmes, novaCapacidade * sizeof(Filme)
        );

        /* Verifica se a realocação funcionou. */
        if (temporario == NULL) {
            return 0;
        }

        *filmes = temporario;
        *capacidade = novaCapacidade;
    }

    /* Adiciona o filme na próxima posição livre. */
    (*filmes)[*quantidade] = novo;

    (*quantidade)++;

    return 1;
}

/* Busca um filme pelo seu ID e retorna sua posição. */
int buscarFilmePorId(Filme* filmes, int quantidade, int id) {

    if (filmes == NULL || quantidade <= 0 || id <= 0) {
        return -1;
    }

    /* Percorre o vetor procurando o ID informado. */
    for (int i = 0; i < quantidade; i++) {

        if (filmes[i].id == id) {
            return i;
        }
    }

    return -1;
}

/* Remove um filme do cadastro utilizando seu ID. */
int removerFilme(Filme* filmes, int* quantidade, int id) {

    if (filmes == NULL || quantidade == NULL ||
        *quantidade <= 0 || id <= 0) {
        return 0;
    }

    /* Procura a posição do filme que será removido. */
    int posicao = buscarFilmePorId(filmes, *quantidade, id);

    if (posicao == -1) {
        return 0;
    }

    /* Desloca os elementos seguintes uma posição para a esquerda. */
    for (int i = posicao; i < *quantidade - 1; i++) {
        filmes[i] = filmes[i + 1];
    }

    (*quantidade)--;

    return 1;
}

/* Atualiza as informações de um filme já cadastrado. */
int atualizarFilme(Filme* filmes, int quantidade,
                   int id, Filme novosDados) {

    if (filmes == NULL || quantidade <= 0 ||
        id <= 0 || !filmeValido(novosDados)) {
        return 0;
    }

    /* Procura o filme que será atualizado. */
    int posicao = buscarFilmePorId(filmes, quantidade, id);

    if (posicao == -1) {
        return 0;
    }

    /* Mantém o ID original, pois ele é a chave do filme. */
    novosDados.id = id;

    /* Substitui os dados antigos pelos novos. */
    filmes[posicao] = novosDados;

    return 1;
}

/* Lista todos os filmes cadastrados. */
void listarFilmes(Filme* filmes, int quantidade) {

    if (filmes == NULL || quantidade <= 0) {
        printf("\nNenhum filme cadastrado.\n");
        return;
    }

    printf("\n========== FILMES CADASTRADOS ==========\n");

    for (int i = 0; i < quantidade; i++) {

        printf("\nID: %d\n", filmes[i].id);
        printf("Titulo: %s\n", filmes[i].titulo);
        printf("Duracao: %d minutos\n", filmes[i].duracao);
        printf("Sinopse: %s\n", filmes[i].sinopse);
        printf("Classificacao: %s\n", filmes[i].classificacao);
        printf("Ano de lancamento: %d\n", filmes[i].anoLanc);
        printf("Situacao: %s\n", filmes[i].situacao);

        printf("---------------------------------------\n");
    }
}

/* Libera a memória utilizada pelo vetor de filmes. */
void liberarFilmes(Filme* filmes) {

    free(filmes);
}

