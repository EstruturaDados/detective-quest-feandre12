#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================================
// Estrutura que representa um cômodo da mansão
// ========================================
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ========================================
// Estrutura da árvore BST para armazenar pistas
// ========================================
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ========================================
// Função: criarSala
// Cria dinamicamente uma nova sala, com ou sem pista
// ========================================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");  // sem pista
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ========================================
// Função: criarNodoPista
// Cria um novo nó para armazenar uma pista
// ========================================
PistaNode* criarNodoPista(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para a pista.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ========================================
// Função: inserirPista
// Insere uma pista na árvore BST em ordem alfabética
// ========================================
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL)
        return criarNodoPista(pista);

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // se for igual, não insere duplicata
    return raiz;
}

// ========================================
// Função: exibirPistas
// Exibe as pistas em ordem alfabética (in-order traversal)
// ========================================
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL)
        return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ========================================
// Função: liberarPistas
// Libera recursivamente a memória da BST de pistas
// ========================================
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// ========================================
// Função: liberarSalas
// Libera recursivamente a memória da árvore da mansão
// ========================================
void liberarSalas(Sala *raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

// ========================================
// Função: explorarSalasComPistas
// Controla a navegação entre salas e coleta de pistas
// ========================================
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se a sala tem uma pista
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista!\n");
            printf("\"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        } else {
            printf("Não há pistas nesta sala.\n");
        }

        printf("\nEscolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("\nEncerrando a exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ========================================
// Função principal: main
// Monta o mapa fixo da mansão e inicia o jogo
// ========================================
int main() {
    // Construção manual do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada", "Um bilhete rasgado: 'a verdade está no porão...'");
    Sala *salaEstar = criarSala("Sala de Estar", "Uma pegada de lama perto da lareira.");
    Sala *cozinha = criarSala("Cozinha", "Uma colher com resquícios de algo brilhante.");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro aberto na página 42.");
    Sala *jardim = criarSala("Jardim", "");
    Sala *quarto = criarSala("Quarto Principal", "Um perfume adocicado no ar.");
    Sala *porao = criarSala("Porão Misterioso", "Uma luva suja de tinta vermelha.");

    // Ligação entre salas (estrutura da árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;
    quarto->esquerda = porao;

    // Árvore de pistas começa vazia
    PistaNode *pistas = NULL;

    printf("===============================================\n");
    printf("   Detective Quest: A Mansão das Pistas\n");
    printf("===============================================\n");
    printf("Você inicia sua investigação no Hall de Entrada...\n");

    // Inicia a exploração com coleta de pistas
    explorarSalasComPistas(hall, &pistas);

    // Exibe todas as pistas coletadas
    printf("\n===== PISTAS COLETADAS =====\n");
    if (pistas == NULL)
        printf("Nenhuma pista foi encontrada!\n");
    else
        exibirPistas(pistas);

    // Libera memória
    liberarSalas(hall);
    liberarPistas(pistas);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}