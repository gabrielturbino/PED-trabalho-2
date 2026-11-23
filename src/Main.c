#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "filme.h"
#include "sessao.h"
#include "ingresso.h"

/* =====================================================
   FUNCOES AUXILIARES DE ENTRADA DE DADOS
   ===================================================== */

/* Le um texto do teclado. */
void lerTexto(char mensagem[], char destino[], int tamanho) {

    printf("%s", mensagem);

    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\nErro na leitura dos dados.\n");
        exit(1);
    }

    /* Remove a quebra de linha. */
    size_t tamanhoTexto = strlen(destino);

    if (tamanhoTexto > 0 && destino[tamanhoTexto - 1] == '\n') {
        destino[tamanhoTexto - 1] = '\0';
    } else {
        /* Descarta caracteres que ultrapassaram o limite. */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

/* Le um numero inteiro e verifica a entrada. */
int lerInteiro(char mensagem[]) {

    char entrada[100];
    char* fim;
    long valor;

    while (1) {

        lerTexto(mensagem, entrada, sizeof(entrada));

        errno = 0;
        valor = strtol(entrada, &fim, 10);

        if (entrada[0] != '\0' && *fim == '\0' &&
            fim != entrada && errno != ERANGE &&
            valor >= INT_MIN && valor <= INT_MAX) {

            return (int)valor;
        }

        printf("Valor invalido! Digite um numero inteiro.\n");
    }
}

/* Le um numero decimal e verifica a entrada. */
float lerReal(char mensagem[]) {

    char entrada[100];
    char* fim;
    float valor;

    while (1) {

        lerTexto(mensagem, entrada, sizeof(entrada));

        errno = 0;
        valor = strtof(entrada, &fim);

        if (entrada[0] != '\0' && *fim == '\0' &&
            fim != entrada && errno != ERANGE &&
            valor >= 0.0f && valor <= 100000000.0f) {

            return valor;
        }

        printf("Valor invalido! Digite um numero positivo.\n");
    }
}

/* =====================================================
   FUNCOES DE ENTRADA DAS ENTIDADES
   ===================================================== */

/* Recebe os dados de um filme. */
Filme lerDadosFilme() {

    Filme filme;

    printf("\n========== CADASTRO DE FILME ==========\n");

    filme.id = lerInteiro("ID do filme: ");

    lerTexto("Titulo: ", filme.titulo, sizeof(filme.titulo));

    filme.duracao = lerInteiro("Duracao em minutos: ");

    lerTexto("Sinopse: ", filme.sinopse, sizeof(filme.sinopse));

    lerTexto("Classificacao indicativa: ",
             filme.classificacao, sizeof(filme.classificacao));

    filme.anoLanc = lerInteiro("Ano de lancamento: ");

    lerTexto("Situacao do filme: ",
             filme.situacao, sizeof(filme.situacao));

    return filme;
}

/* Recebe os dados de uma sessao. */
Sessao lerDadosSessao() {

    Sessao sessao;

    printf("\n========== CADASTRO DE SESSAO ==========\n");

    sessao.ident = lerInteiro("Codigo da sessao: ");

    lerTexto("Data (AAAA-MM-DD): ",
             sessao.data, sizeof(sessao.data));

    lerTexto("Horario de inicio (HH:MM:SS): ",
             sessao.horInicio, sizeof(sessao.horInicio));

    lerTexto("Horario de termino (HH:MM:SS): ",
             sessao.horTerm, sizeof(sessao.horTerm));

    lerTexto("Situacao da sessao: ",
             sessao.situacao, sizeof(sessao.situacao));

    sessao.idFilme = lerInteiro("ID do filme: ");

    sessao.cod = lerInteiro("Codigo da sala: ");

    return sessao;
}

/* Recebe os dados de um ingresso. */
Ingresso lerDadosIngresso() {

    Ingresso ingresso;

    printf("\n========== CADASTRO DE INGRESSO ==========\n");

    ingresso.ident = lerInteiro("Codigo da sessao: ");

    lerTexto("Assento (exemplo: A01): ",
             ingresso.assento, sizeof(ingresso.assento));

    lerTexto("Data (AAAA-MM-DD): ",
             ingresso.data, sizeof(ingresso.data));

    ingresso.valor = lerReal("Valor do ingresso: R$ ");

    lerTexto("Situacao do ingresso: ",
             ingresso.situacao, sizeof(ingresso.situacao));

    lerTexto("CPF do cliente (somente numeros): ",
             ingresso.cpf, sizeof(ingresso.cpf));

    return ingresso;
}

/* =====================================================
   FUNCOES AUXILIARES DE RELACIONAMENTO
   ===================================================== */

/* Verifica se um filme possui sessoes cadastradas. */
int filmePossuiSessao(Sessao* sessoes, int quantidade, int idFilme) {

    for (int i = 0; i < quantidade; i++) {

        if (sessoes[i].idFilme == idFilme) {
            return 1;
        }
    }

    return 0;
}

/* Verifica se uma sessao possui ingressos cadastrados. */
int sessaoPossuiIngresso(Ingresso* ingressos,
                         int quantidade, int ident) {

    for (int i = 0; i < quantidade; i++) {

        if (ingressos[i].ident == ident) {
            return 1;
        }
    }

    return 0;
}

/* =====================================================
   OPERACOES COM FILMES
   ===================================================== */

/* Cadastra um novo filme. */
void cadastrarFilme(Filme** filmes, int* quantidade, int* capacidade) {

    Filme novo = lerDadosFilme();

    if (inserirFilme(filmes, quantidade, capacidade, novo)) {
        printf("\nFilme cadastrado com sucesso!\n");
    } else {
        printf("\nNao foi possivel cadastrar o filme.\n");
        printf("Verifique os dados ou se o ID ja existe.\n");
    }
}

/* Busca e mostra um filme pelo ID. */
void consultarFilme(Filme* filmes, int quantidade) {

    int id = lerInteiro("\nDigite o ID do filme: ");

    int posicao = buscarFilmePorId(filmes, quantidade, id);

    if (posicao == -1) {
        printf("\nFilme nao encontrado.\n");
    } else {
        printf("\nFilme encontrado:\n");
        listarFilmes(&filmes[posicao], 1);
    }
}

/* Atualiza um filme. */
void modificarFilme(Filme* filmes, int quantidade) {

    int id = lerInteiro("\nDigite o ID do filme que deseja atualizar: ");

    if (buscarFilmePorId(filmes, quantidade, id) == -1) {
        printf("\nFilme nao encontrado.\n");
        return;
    }

    Filme novosDados = lerDadosFilme();

    novosDados.id = id;

    if (atualizarFilme(filmes, quantidade, id, novosDados)) {
        printf("\nFilme atualizado com sucesso!\n");
    } else {
        printf("\nNao foi possivel atualizar o filme.\n");
    }
}

/* Remove um filme. */
void excluirFilme(Filme* filmes, int* quantidade,
                  Sessao* sessoes, int qtdSessoes) {

    int id = lerInteiro("\nDigite o ID do filme que deseja remover: ");

    if (buscarFilmePorId(filmes, *quantidade, id) == -1) {
        printf("\nFilme nao encontrado.\n");
        return;
    }

    if (filmePossuiSessao(sessoes, qtdSessoes, id)) {
        printf("\nNao e possivel remover este filme.\n");
        printf("Existem sessoes cadastradas para ele.\n");
        return;
    }

    if (removerFilme(filmes, quantidade, id)) {
        printf("\nFilme removido com sucesso!\n");
    } else {
        printf("\nErro ao remover o filme.\n");
    }
}

/* =====================================================
   OPERACOES COM SESSOES
   ===================================================== */

/* Cadastra uma nova sessao. */
void cadastrarSessao(Sessao** sessoes, int* quantidade,
                     int* capacidade, Filme* filmes, int qtdFilmes) {

    Sessao nova = lerDadosSessao();

    /* Verifica se o filme informado existe. */
    if (buscarFilmePorId(filmes, qtdFilmes, nova.idFilme) == -1) {
        printf("\nErro: o filme informado nao esta cadastrado.\n");
        return;
    }

    if (inserirSessao(sessoes, quantidade, capacidade, nova)) {
        printf("\nSessao cadastrada com sucesso!\n");
    } else {
        printf("\nNao foi possivel cadastrar a sessao.\n");
        printf("Verifique os dados ou se o codigo ja existe.\n");
    }
}

/* Busca e mostra uma sessao pelo ID. */
void consultarSessao(Sessao* sessoes, int quantidade) {

    int id = lerInteiro("\nDigite o codigo da sessao: ");

    int posicao = buscarSessaoPorId(sessoes, quantidade, id);

    if (posicao == -1) {
        printf("\nSessao nao encontrada.\n");
    } else {
        printf("\nSessao encontrada:\n");
        listarSessoes(&sessoes[posicao], 1);
    }
}

/* Atualiza uma sessao. */
void modificarSessao(Sessao* sessoes, int quantidade,
                     Filme* filmes, int qtdFilmes) {

    int id = lerInteiro("\nDigite o codigo da sessao que deseja atualizar: ");

    if (buscarSessaoPorId(sessoes, quantidade, id) == -1) {
        printf("\nSessao nao encontrada.\n");
        return;
    }

    Sessao novosDados = lerDadosSessao();

    /* Mantem o identificador original. */
    novosDados.ident = id;

    if (buscarFilmePorId(filmes, qtdFilmes, novosDados.idFilme) == -1) {
        printf("\nErro: o filme informado nao esta cadastrado.\n");
        return;
    }

    if (atualizarSessao(sessoes, quantidade, id, novosDados)) {
        printf("\nSessao atualizada com sucesso!\n");
    } else {
        printf("\nNao foi possivel atualizar a sessao.\n");
    }
}

/* Remove uma sessao. */
void excluirSessao(Sessao* sessoes, int* quantidade,
                   Ingresso* ingressos, int qtdIngressos) {

    int id = lerInteiro("\nDigite o codigo da sessao que deseja remover: ");

    if (buscarSessaoPorId(sessoes, *quantidade, id) == -1) {
        printf("\nSessao nao encontrada.\n");
        return;
    }

    if (sessaoPossuiIngresso(ingressos, qtdIngressos, id)) {
        printf("\nNao e possivel remover esta sessao.\n");
        printf("Existem ingressos vinculados a ela.\n");
        return;
    }

    if (removerSessao(sessoes, quantidade, id)) {
        printf("\nSessao removida com sucesso!\n");
    } else {
        printf("\nErro ao remover a sessao.\n");
    }
}

/* =====================================================
   OPERACOES COM INGRESSOS
   ===================================================== */

/* Cadastra um novo ingresso. */
void cadastrarIngresso(Ingresso** ingressos, int* quantidade,
                       int* capacidade, Sessao* sessoes, int qtdSessoes) {

    Ingresso novo = lerDadosIngresso();

    /* Verifica se a sessao informada existe. */
    if (buscarSessaoPorId(sessoes, qtdSessoes, novo.ident) == -1) {
        printf("\nErro: a sessao informada nao esta cadastrada.\n");
        return;
    }

    if (inserirIngresso(ingressos, quantidade, capacidade, novo)) {
        printf("\nIngresso cadastrado com sucesso!\n");
    } else {
        printf("\nNao foi possivel cadastrar o ingresso.\n");
        printf("Verifique os dados ou se o assento ja esta ocupado.\n");
    }
}

/* Busca e mostra um ingresso pela sessao e pelo assento. */
void consultarIngresso(Ingresso* ingressos, int quantidade) {

    int ident = lerInteiro("\nDigite o codigo da sessao: ");

    char assento[11];

    lerTexto("Digite o assento: ", assento, sizeof(assento));

    int posicao = buscarIngresso(
        ingressos, quantidade, ident, assento
    );

    if (posicao == -1) {
        printf("\nIngresso nao encontrado.\n");
    } else {
        printf("\nIngresso encontrado:\n");
        listarIngressos(&ingressos[posicao], 1);
    }
}

/* Atualiza um ingresso. */
void modificarIngresso(Ingresso* ingressos, int quantidade,
                       Sessao* sessoes, int qtdSessoes) {

    int ident = lerInteiro("\nDigite o codigo da sessao: ");

    char assento[11];

    lerTexto("Digite o assento do ingresso que deseja atualizar: ",
             assento, sizeof(assento));

    if (buscarIngresso(ingressos, quantidade, ident, assento) == -1) {
        printf("\nIngresso nao encontrado.\n");
        return;
    }

    Ingresso novosDados = lerDadosIngresso();

    /* Verifica se a nova sessao existe. */
    if (buscarSessaoPorId(sessoes, qtdSessoes, novosDados.ident) == -1) {
        printf("\nErro: a sessao informada nao esta cadastrada.\n");
        return;
    }

    if (atualizarIngresso(
        ingressos, quantidade, ident, assento, novosDados)) {

        printf("\nIngresso atualizado com sucesso!\n");

    } else {
        printf("\nNao foi possivel atualizar o ingresso.\n");
        printf("Verifique se o novo assento ja esta ocupado.\n");
    }
}

/* Remove um ingresso. */
void excluirIngresso(Ingresso* ingressos, int* quantidade) {

    int ident = lerInteiro("\nDigite o codigo da sessao: ");

    char assento[11];

    lerTexto("Digite o assento do ingresso que deseja remover: ",
             assento, sizeof(assento));

    if (removerIngresso(ingressos, quantidade, ident, assento)) {
        printf("\nIngresso removido com sucesso!\n");
    } else {
        printf("\nIngresso nao encontrado.\n");
    }
}

/* =====================================================
   MENU PRINCIPAL
   ===================================================== */

void mostrarMenu() {

    printf("\n");
    printf("============================================\n");
    printf("       SISTEMA DE GERENCIAMENTO DE CINEMA   \n");
    printf("============================================\n");

    printf("\n----------- FILMES -----------\n");
    printf(" 1 - Cadastrar filme\n");
    printf(" 2 - Listar filmes\n");
    printf(" 3 - Buscar filme por ID\n");
    printf(" 4 - Atualizar filme\n");
    printf(" 5 - Remover filme\n");

    printf("\n----------- SESSOES ----------\n");
    printf(" 6 - Cadastrar sessao\n");
    printf(" 7 - Listar sessoes\n");
    printf(" 8 - Buscar sessao por ID\n");
    printf(" 9 - Atualizar sessao\n");
    printf("10 - Remover sessao\n");

    printf("\n----------- INGRESSOS --------\n");
    printf("11 - Cadastrar ingresso\n");
    printf("12 - Listar ingressos\n");
    printf("13 - Buscar ingresso\n");
    printf("14 - Atualizar ingresso\n");
    printf("15 - Remover ingresso\n");

    printf("\n 0 - Sair\n");

    printf("============================================\n");
}

/* =====================================================
   FUNCAO PRINCIPAL
   ===================================================== */

int main() {

    /* Quantidade inicial de elementos. */
    int qtdFilmes = 0;
    int qtdSessoes = 0;
    int qtdIngressos = 0;

    /* Capacidade inicial dos vetores. */
    int capFilmes = 2;
    int capSessoes = 2;
    int capIngressos = 2;

    /* Criacao dos tres vetores dinamicos. */
    Filme* filmes = criarFilmes(capFilmes);
    Sessao* sessoes = criarSessoes(capSessoes);
    Ingresso* ingressos = criarIngressos(capIngressos);

    /* Verifica se todas as alocacoes funcionaram. */
    if (filmes == NULL || sessoes == NULL || ingressos == NULL) {

        printf("\nErro ao alocar memoria para o sistema.\n");

        liberarFilmes(filmes);
        liberarSessoes(sessoes);
        liberarIngressos(ingressos);

        return 1;
    }

    int opcao;

    do {

        mostrarMenu();

        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {

            /* FILMES */

            case 1:
                cadastrarFilme(&filmes, &qtdFilmes, &capFilmes);
                break;

            case 2:
                listarFilmes(filmes, qtdFilmes);
                break;

            case 3:
                consultarFilme(filmes, qtdFilmes);
                break;

            case 4:
                modificarFilme(filmes, qtdFilmes);
                break;

            case 5:
                excluirFilme(filmes, &qtdFilmes, sessoes, qtdSessoes);
                break;

            /* SESSOES */

            case 6:
                cadastrarSessao(
                    &sessoes, &qtdSessoes, &capSessoes,
                    filmes, qtdFilmes
                );
                break;

            case 7:
                listarSessoes(sessoes, qtdSessoes);
                break;

            case 8:
                consultarSessao(sessoes, qtdSessoes);
                break;

            case 9:
                modificarSessao(
                    sessoes, qtdSessoes, filmes, qtdFilmes
                );
                break;

            case 10:
                excluirSessao(
                    sessoes, &qtdSessoes, ingressos, qtdIngressos
                );
                break;

            /* INGRESSOS */

            case 11:
                cadastrarIngresso(
                    &ingressos, &qtdIngressos, &capIngressos,
                    sessoes, qtdSessoes
                );
                break;

            case 12:
                listarIngressos(ingressos, qtdIngressos);
                break;

            case 13:
                consultarIngresso(ingressos, qtdIngressos);
                break;

            case 14:
                modificarIngresso(
                    ingressos, qtdIngressos, sessoes, qtdSessoes
                );
                break;

            case 15:
                excluirIngresso(ingressos, &qtdIngressos);
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    /* Libera toda a memoria utilizada pelo programa. */
    liberarFilmes(filmes);
    liberarSessoes(sessoes);
    liberarIngressos(ingressos);

    printf("Memoria liberada com sucesso!\n");
    printf("Sistema encerrado. Ate mais!\n");

    return 0;
}

