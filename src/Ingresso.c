#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#include "ingresso.h"

/* Verifica se os dados de um ingresso são válidos. */
static int ingressoValido(Ingresso ingresso) {

    if (ingresso.ident <= 0 || ingresso.valor < 0.0f) {
        return 0;
    }

    /* Verifica se os campos de texto possuem final de string. */
    if (memchr(ingresso.assento, '\0', sizeof(ingresso.assento)) == NULL ||
        memchr(ingresso.data, '\0', sizeof(ingresso.data)) == NULL ||
        memchr(ingresso.situacao, '\0', sizeof(ingresso.situacao)) == NULL ||
        memchr(ingresso.cpf, '\0', sizeof(ingresso.cpf)) == NULL) {
        return 0;
    }

    /* O assento não pode estar vazio. */
    if (ingresso.assento[0] == '\0') {
        return 0;
    }

    /* O CPF deve conter 11 dígitos. */
    if (strlen(ingresso.cpf) != 11) {
        return 0;
    }

    for (int i = 0; i < 11; i++) {
        if (ingresso.cpf[i] < '0' || ingresso.cpf[i] > '9') {
            return 0;
        }
    }

    /* Verifica o formato da data AAAA-MM-DD. */
    if (strlen(ingresso.data) != 10 ||
        ingresso.data[4] != '-' ||
        ingresso.data[7] != '-') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 &&
            (ingresso.data[i] < '0' || ingresso.data[i] > '9')) {
            return 0;
        }
    }

    int ano, mes, dia;

    sscanf(ingresso.data, "%4d-%2d-%2d", &ano, &mes, &dia);

    if (ano <= 0 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        return 0;
    }

    return 1;
}

/* Cria o espaço na memória para armazenar os ingressos. */
Ingresso* criarIngressos(int capacidade) {

    if (capacidade <= 0 ||
        (size_t)capacidade > SIZE_MAX / sizeof(Ingresso)) {
        return NULL;
    }

    Ingresso* ingressos = malloc(capacidade * sizeof(Ingresso));

    return ingressos;
}

/* Insere um novo ingresso no cadastro. */
int inserirIngresso(Ingresso** ingressos, int* quantidade,
                    int* capacidade, Ingresso novo) {

    if (ingressos == NULL || quantidade == NULL ||
        capacidade == NULL || !ingressoValido(novo)) {
        return 0;
    }

    if (*quantidade < 0 || *capacidade < 0 ||
        *quantidade > *capacidade) {
        return 0;
    }

    if (*capacidade > 0 && *ingressos == NULL) {
        return 0;
    }

    /* Impede dois ingressos para o mesmo assento na mesma sessão. */
    if (buscarIngresso(*ingressos, *quantidade,
                       novo.ident, novo.assento) != -1) {
        return 0;
    }

    /* Aumenta a capacidade caso o vetor esteja cheio. */
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

        if ((size_t)novaCapacidade > SIZE_MAX / sizeof(Ingresso)) {
            return 0;
        }

        Ingresso* temporario = realloc(
            *ingressos, novaCapacidade * sizeof(Ingresso)
        );

        if (temporario == NULL) {
            return 0;
        }

        *ingressos = temporario;
        *capacidade = novaCapacidade;
    }

    /* Adiciona o ingresso na próxima posição livre. */
    (*ingressos)[*quantidade] = novo;

    (*quantidade)++;

    return 1;
}

/* Busca um ingresso pelo código da sessão e pelo assento. */
int buscarIngresso(Ingresso* ingressos, int quantidade,
                   int ident, char assento[]) {

    if (ingressos == NULL || quantidade <= 0 ||
        ident <= 0 || assento == NULL || assento[0] == '\0') {
        return -1;
    }

    /* Percorre o vetor procurando a sessão e o assento. */
    for (int i = 0; i < quantidade; i++) {

        if (ingressos[i].ident == ident &&
            strcmp(ingressos[i].assento, assento) == 0) {
            return i;
        }
    }

    return -1;
}

/* Remove um ingresso utilizando a sessão e o assento. */
int removerIngresso(Ingresso* ingressos, int* quantidade,
                    int ident, char assento[]) {

    if (ingressos == NULL || quantidade == NULL ||
        *quantidade <= 0 || ident <= 0 || assento == NULL) {
        return 0;
    }

    /* Procura a posição do ingresso que será removido. */
    int posicao = buscarIngresso(
        ingressos, *quantidade, ident, assento
    );

    if (posicao == -1) {
        return 0;
    }

    /* Desloca os elementos seguintes uma posição para a esquerda. */
    for (int i = posicao; i < *quantidade - 1; i++) {
        ingressos[i] = ingressos[i + 1];
    }

    (*quantidade)--;

    return 1;
}

/* Atualiza as informações de um ingresso cadastrado. */
int atualizarIngresso(Ingresso* ingressos, int quantidade,
                      int ident, char assento[], Ingresso novosDados) {

    if (ingressos == NULL || quantidade <= 0 ||
        ident <= 0 || assento == NULL ||
        !ingressoValido(novosDados)) {
        return 0;
    }

    /* Procura o ingresso que será atualizado. */
    int posicao = buscarIngresso(
        ingressos, quantidade, ident, assento
    );

    if (posicao == -1) {
        return 0;
    }

    /* Impede que a atualização gere outro ingresso duplicado. */
    int outraPosicao = buscarIngresso(
        ingressos, quantidade,
        novosDados.ident, novosDados.assento
    );

    if (outraPosicao != -1 && outraPosicao != posicao) {
        return 0;
    }

    /* Substitui os dados antigos pelos novos. */
    ingressos[posicao] = novosDados;

    return 1;
}

/* Lista todos os ingressos cadastrados. */
void listarIngressos(Ingresso* ingressos, int quantidade) {

    if (ingressos == NULL || quantidade <= 0) {
        printf("\nNenhum ingresso cadastrado.\n");
        return;
    }

    printf("\n========== INGRESSOS CADASTRADOS ==========\n");

    for (int i = 0; i < quantidade; i++) {

        printf("\nCodigo da sessao: %d\n", ingressos[i].ident);
        printf("Assento: %s\n", ingressos[i].assento);
        printf("Data: %s\n", ingressos[i].data);
        printf("Valor: R$ %.2f\n", ingressos[i].valor);
        printf("Situacao: %s\n", ingressos[i].situacao);
        printf("CPF do cliente: %s\n", ingressos[i].cpf);

        printf("-------------------------------------------\n");
    }
}

/* Libera a memória utilizada pelo vetor de ingressos. */
void liberarIngressos(Ingresso* ingressos) {

    free(ingressos);
}

