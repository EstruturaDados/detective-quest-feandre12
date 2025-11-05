#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===============================
// Estrutura da Sala (nó da árvore)
// ===============================
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ===================================
// Função: criarSala
// Cria dinamicamente uma nova sala,
// define seu nome e inicializa os ponteiros
// ===================================
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ===================================
// Função: explorarSalas
// Permite ao jogador navegar pela árvore
// Escolhendo entre esquerda (e) ou direita (d)
// até chegar a um cômodo sem saídas
// ===================================
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Caso a sala não tenha mais saídas
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim desta rota da mansão!\n");
            break;
        }

        printf("Escolha o caminho:\n");
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
            printf("\nSaindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ===================================
// Função: liberarSalas
// Libera recursivamente toda a memória da árvore
// ===================================
void liberarSalas(Sala *raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

// ===================================
// Função principal: main
// Monta o mapa da mansão manualmente e inicia o jogo
// ===================================
int main() {
    // Montagem manual da árvore (mansão)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *quarto = criarSala("Quarto Principal");
    Sala *porao = criarSala("Porão Misterioso");

    // Estrutura da mansão (árvore)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;
    quarto->esquerda = porao;

    printf("=== Detective Quest: A Mansão Misteriosa ===\n");
    printf("Bem-vindo(a)! Você iniciará sua exploração pelo Hall de Entrada.\n");

    // Inicia a exploração interativa
    explorarSalas(hall);

    // Libera memória alocada
    liberarSalas(hall);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}