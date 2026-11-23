#ifndef SESSAO_H
#define SESSAO_H

// Estrutura que guarda as informações de uma sessão.
typedef struct {
    int ident;                  // Código que identifica a sessão
    char data[11];              // Data da sessão (AAAA-MM-DD)
    char horInicio[9];          // Horário de início (HH:MM:SS)
    char horTerm[9];            // Horário de término (HH:MM:SS)
    char situacao[31];          // Situação da sessão
    int idFilme;                // Código do filme exibido
    int cod;                    // Código da sala onde ocorre a sessão
} Sessao;


// Cria o espaço na memória para guardar as sessões.
// Recebe a capacidade inicial e retorna o endereço da estrutura.
// Retorna NULL se não conseguir reservar a memória.
Sessao* criarSessoes(int capacidade);


// Adiciona uma nova sessão ao cadastro.
// Recebe a lista de sessões, a quantidade atual, a capacidade e a nova sessão.
// Retorna 1 se conseguir adicionar ou 0 se ocorrer algum erro.
int inserirSessao(Sessao** sessoes, int* quantidade,
                  int* capacidade, Sessao nova);


// Procura uma sessão pelo seu código (ident).
// Recebe a lista de sessões, a quantidade e o código procurado.
// Retorna a posição da sessão na lista ou -1 se não encontrar.
int buscarSessaoPorId(Sessao* sessoes, int quantidade, int ident);


// Remove uma sessão do cadastro usando seu código.
// Recebe a lista de sessões, a quantidade atual e o código da sessão.
// Retorna 1 se remover ou 0 se a sessão não for encontrada.
int removerSessao(Sessao* sessoes, int* quantidade, int ident);


// Altera as informações de uma sessão já cadastrada.
// Recebe a lista, a quantidade, o código e os novos dados da sessão.
// Retorna 1 se atualizar ou 0 se não encontrar a sessão.
int atualizarSessao(Sessao* sessoes, int quantidade,
                    int ident, Sessao novosDados);


// Mostra todas as sessões que estão cadastradas.
// Recebe a lista de sessões e a quantidade de registros.
// Não retorna nenhum valor.
void listarSessoes(Sessao* sessoes, int quantidade);


// Libera o espaço da memória que foi utilizado pela lista de sessões.
// Recebe a lista de sessões.
// Não retorna nenhum valor.
void liberarSessoes(Sessao* sessoes);

#endif
