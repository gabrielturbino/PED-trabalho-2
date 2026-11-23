#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

/* Inclui a definição da estrutura Sessao e seus protótipos. */
#include "sessao.h"

/* Verifica se os dados de uma sessão são válidos. */
static int sessaoValida(Sessao sessao) {

    if (sessao.ident <= 0 || sessao.idFilme <= 0 || sessao.cod <= 0) {
        return 0;
    }

    /* Verifica se os campos de texto possuem final de string. */
    if (memchr(sessao.data, '\0', sizeof(sessao.data)) == NULL ||
        memchr(sessao.horInicio, '\0', sizeof(sessao.horInicio)) == NULL ||
        memchr(sessao.horTerm, '\0', sizeof(sessao.horTerm)) == NULL ||
        memchr(sessao.situacao, '\0', sizeof(sessao.situacao)) == NULL) {
        return 0;
    }

    /* Verifica o formato da data AAAA-MM-DD. */
    if (strlen(sessao.data) != 10 ||
        sessao.data[4] != '-' ||
        sessao.data[7] != '-') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 &&
            (sessao.data[i] < '0' || sessao.data[i] > '9')) {
            return 0;
        }
    }

    int ano, mes, dia;

    sscanf(sessao.data, "%4d-%2d-%2d", &ano, &mes, &dia);

    if (ano <= 0 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        return 0;
    }

    /* Verifica o formato do horário HH:MM:SS. */
    if (strlen(sessao.horInicio) != 8 ||
        sessao.horInicio[2] != ':' ||
        sessao.horInicio[5] != ':' ||
        strlen(sessao.horTerm) != 8 ||
        sessao.horTerm[2] != ':' ||
        sessao.horTerm[5] != ':') {
        return 0;
    }

    /* Verifica se os números dos horários são válidos. */
    for (int i = 0; i < 8; i++) {

        if (i == 2 || i == 5) {
            continue;
        }

        if (sessao.horInicio[i] < '0' ||
            sessao.horInicio[i] > '9' ||
            sessao.horTerm[i] < '0' ||
            sessao.horTerm[i] > '9') {
            return 0;
        }
    }

    int horaInicio, minutoInicio, segundoInicio;
    int horaTerm, minutoTerm, segundoTerm;

    sscanf(sessao.horInicio, "%2d:%2d:%2d",
           &horaInicio, &minutoInicio, &segundoInicio);

    sscanf(sessao.horTerm, "%2d:%2d:%2d",
           &horaTerm, &minutoTerm, &segundoTerm);

    if (horaInicio > 23 || minutoInicio > 59 || segundoInicio > 59 ||
        horaTerm > 23 || minutoTerm > 59 || segundoTerm > 59) {
        return 0;
    }

    /* A sessão precisa terminar depois de começar. */
    int inicio = horaInicio * 3600 + minutoInicio * 60 + segundoInicio;
    int termino = horaTerm * 3600 + minutoTerm * 60 + segundoTerm;

    if (termino <= inicio) {
        return 0;
    }

    return 1;
}

/* Cria o espaço na memória para armazenar as sessões. */
Sessao* criarSessoes(int capacidade) {

    if (capacidade <= 0 ||
        (size_t)capacidade > SIZE_MAX / sizeof(Sessao)) {
        return NULL;
    }

    Sessao* sessoes = malloc(capacidade * sizeof(Sessao));

    return sessoes;
}

/* Insere uma nova sessão no cadastro. */
int inserirSessao(Sessao** sessoes, int* quantidade,
                  int* capacidade, Sessao nova) {

    if (sessoes == NULL || quantidade == NULL ||
        capacidade == NULL || !sessaoValida(nova)) {
        return 0;
    }

    if (*quantidade < 0 || *capacidade < 0 ||
        *quantidade > *capacidade) {
        return 0;
    }

    if (*capacidade > 0 && *sessoes == NULL) {
        return 0;
    }

    /* Impede o cadastro de duas sessões com o mesmo ID. */
    if (buscarSessaoPorId(*sessoes, *quantidade, nova.ident) != -1) {
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

        if ((size_t)novaCapacidade > SIZE_MAX / sizeof(Sessao)) {
            return 0;
        }

        Sessao* temporario = realloc(
            *sessoes, novaCapacidade * sizeof(Sessao)
        );

        /* Verifica se a realocação funcionou. */
        if (temporario == NULL) {
            return 0;
        }

        *sessoes = temporario;
        *capacidade = novaCapacidade;
    }

    /* Adiciona a sessão na próxima posição livre. */
    (*sessoes)[*quantidade] = nova;

    (*quantidade)++;

    return 1;
}

/* Busca uma sessão pelo seu código identificador. */
int buscarSessaoPorId(Sessao* sessoes, int quantidade, int ident) {

    if (sessoes == NULL || quantidade <= 0 || ident <= 0) {
        return -1;
    }

    /* Percorre o vetor procurando o identificador informado. */
    for (int i = 0; i < quantidade; i++) {

        if (sessoes[i].ident == ident) {
            return i;
        }
    }

    return -1;
}

/* Remove uma sessão do cadastro utilizando seu identificador. */
int removerSessao(Sessao* sessoes, int* quantidade, int ident) {

    if (sessoes == NULL || quantidade == NULL ||
        *quantidade <= 0 || ident <= 0) {
        return 0;
    }

    /* Procura a posição da sessão que será removida. */
    int posicao = buscarSessaoPorId(sessoes, *quantidade, ident);

    if (posicao == -1) {
        return 0;
    }

    /* Desloca os elementos seguintes uma posição para a esquerda. */
    for (int i = posicao; i < *quantidade - 1; i++) {
        sessoes[i] = sessoes[i + 1];
    }

    (*quantidade)--;

    return 1;
}

/* Atualiza as informações de uma sessão já cadastrada. */
int atualizarSessao(Sessao* sessoes, int quantidade,
                    int ident, Sessao novosDados) {

    if (sessoes == NULL || quantidade <= 0 ||
        ident <= 0 || !sessaoValida(novosDados)) {
        return 0;
    }

    /* Procura a sessão que será atualizada. */
    int posicao = buscarSessaoPorId(sessoes, quantidade, ident);

    if (posicao == -1) {
        return 0;
    }

    /* Mantém o identificador original da sessão. */
    novosDados.ident = ident;

    /* Substitui os dados antigos pelos novos. */
    sessoes[posicao] = novosDados;

    return 1;
}

/* Lista todas as sessões cadastradas. */
void listarSessoes(Sessao* sessoes, int quantidade) {

    if (sessoes == NULL || quantidade <= 0) {
        printf("\nNenhuma sessao cadastrada.\n");
        return;
    }

    printf("\n========== SESSOES CADASTRADAS ==========\n");

    for (int i = 0; i < quantidade; i++) {

        printf("\nCodigo da sessao: %d\n", sessoes[i].ident);
        printf("Data: %s\n", sessoes[i].data);
        printf("Horario de inicio: %s\n", sessoes[i].horInicio);
        printf("Horario de termino: %s\n", sessoes[i].horTerm);
        printf("Situacao: %s\n", sessoes[i].situacao);
        printf("Codigo do filme: %d\n", sessoes[i].idFilme);
        printf("Codigo da sala: %d\n", sessoes[i].cod);

        printf("----------------------------------------\n");
    }
}

/* Libera a memória utilizada pelo vetor de sessões. */
void liberarSessoes(Sessao* sessoes) {

    free(sessoes);
}

