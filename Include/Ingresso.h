#ifndef INGRESSO_H
#define INGRESSO_H

// Estrutura que guarda as informações de um ingresso.
typedef struct {
    int ident;                  // Código da sessão
    char assento[11];           // Assento do ingresso
    char data[11];              // Data do ingresso (AAAA-MM-DD)
    float valor;                // Valor pago pelo ingresso
    char situacao[31];          // Situação do ingresso
    char cpf[12];               // CPF do cliente
} Ingresso;


// Cria o espaço na memória para guardar os ingressos.
// Recebe a capacidade inicial e retorna o endereço da estrutura.
// Retorna NULL se não conseguir reservar a memória.
Ingresso* criarIngressos(int capacidade);


// Adiciona um novo ingresso ao cadastro.
// Recebe a lista de ingressos, a quantidade atual, a capacidade e o novo ingresso.
// Retorna 1 se conseguir adicionar ou 0 se ocorrer algum erro.
int inserirIngresso(Ingresso** ingressos, int* quantidade,
                    int* capacidade, Ingresso novo);


// Procura um ingresso pelo código da sessão e pelo assento.
// Recebe a lista de ingressos, a quantidade, o código da sessão e o assento.
// Retorna a posição do ingresso na lista ou -1 se não encontrar.
int buscarIngresso(Ingresso* ingressos, int quantidade,
                   int ident, char assento[]);


// Remove um ingresso do cadastro usando o código da sessão e o assento.
// Recebe a lista de ingressos, a quantidade atual, o código da sessão e o assento.
// Retorna 1 se remover ou 0 se o ingresso não for encontrado.
int removerIngresso(Ingresso* ingressos, int* quantidade,
                    int ident, char assento[]);


// Altera as informações de um ingresso já cadastrado.
// Recebe a lista, a quantidade, o código da sessão, o assento e os novos dados.
// Retorna 1 se atualizar ou 0 se não encontrar o ingresso.
int atualizarIngresso(Ingresso* ingressos, int quantidade,
                      int ident, char assento[], Ingresso novosDados);


// Mostra todos os ingressos que estão cadastrados.
// Recebe a lista de ingressos e a quantidade de registros.
// Não retorna nenhum valor.
void listarIngressos(Ingresso* ingressos, int quantidade);


// Libera o espaço da memória que foi utilizado pela lista de ingressos.
// Recebe a lista de ingressos.
// Não retorna nenhum valor.
void liberarIngressos(Ingresso* ingressos);

#endif
