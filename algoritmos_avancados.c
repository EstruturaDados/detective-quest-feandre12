#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 10

// =======================================================
// Estrutura que representa um cômodo da mansão (árvore binária)
// =======================================================
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// =======================================================
// Estrutura da BST de pistas
// =======================================================
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// =======================================================
// Estrutura da tabela hash (associa pista → suspeito)
// =======================================================
typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

HashItem* tabelaHash[TAM_HASH];

// =======================================================
// Função hash simples (soma dos caracteres da pista)
// =======================================================
int hashFunction(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += tolower(chave[i]);
    return soma % TAM_HASH;
}

// =======================================================
// criarSala() – cria dinamicamente um cômodo
// =======================================================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// =======================================================
// inserirPista() – insere uma nova pista na BST (ordenada)
// =======================================================
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// =======================================================
// inserirNaHash() – associa pista a um suspeito
// =======================================================
void inserirNaHash(const char *pista, const char *suspeito) {
    int idx = hashFunction(pista);
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

// =======================================================
// encontrarSuspeito() – busca o suspeito correspondente à pista
// =======================================================
char* encontrarSuspeito(const char *pista) {
    int idx = hashFunction(pista);
    HashItem *atual = tabelaHash[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}

// =======================================================
// exibirPistas() – imprime as pistas em ordem alfabética
// =======================================================
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// =======================================================
// contarPistasDoSuspeito() – percorre BST e conta quantas pistas
// estão associadas ao suspeito acusado
// =======================================================
int contarPistasDoSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    if (strcmp(encontrarSuspeito(raiz->pista), suspeito) == 0)
        count = 1;
    return count +
           contarPistasDoSuspeito(raiz->esquerda, suspeito) +
           contarPistasDoSuspeito(raiz->direita, suspeito);
}

// =======================================================
// verificarSuspeitoFinal() – conduz à fase de julgamento
// =======================================================
void verificarSuspeitoFinal(PistaNode *raiz) {
    char acusado[50];
    printf("\n===== JULGAMENTO FINAL =====\n");
    printf("Digite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistasDoSuspeito(raiz, acusado);

    printf("\nPistas que apontam para %s: %d\n", acusado, qtd);
    if (qtd >= 2) {
        printf("Acusação bem fundamentada! %s é o verdadeiro culpado!\n", acusado);
    } else {
        printf("Pistas insuficientes... talvez você tenha acusado a pessoa errada.\n");
    }
}

// =======================================================
// explorarSalas() – navegação pela mansão e coleta de pistas
// =======================================================
void explorarSalas(Sala *atual, PistaNode **raizPistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista!\n\"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        } else {
            printf("Nenhuma pista aqui.\n");
        }

        printf("\nEscolha o caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita) printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita)
            atual = atual->direita;
        else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

// =======================================================
// liberarMemoria() – libera as estruturas alocadas
// =======================================================
void liberarSalas(Sala *raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashItem *atual = tabelaHash[i];
        while (atual) {
            HashItem *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

// =======================================================
// Função principal – monta o jogo completo
// =======================================================
int main() {
    // Mapa fixo da mansão (árvore binária)
    Sala *hall = criarSala("Hall de Entrada", "Um bilhete rasgado com a inicial 'C'.");
    Sala *salaEstar = criarSala("Sala de Estar", "Uma luva suja de tinta.");
    Sala *cozinha = criarSala("Cozinha", "Um colar caro deixado sobre a mesa.");
    Sala *biblioteca = criarSala("Biblioteca", "Uma anotação sobre veneno.");
    Sala *jardim = criarSala("Jardim", "");
    Sala *quarto = criarSala("Quarto Principal", "Um perfume raro de uso masculino.");
    Sala *porao = criarSala("Porão Misterioso", "Uma chave com o brasão da família.");

    // Conexões da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;
    quarto->esquerda = porao;

    // Criação da tabela hash (pistas → suspeitos)
    inserirNaHash("Um bilhete rasgado com a inicial 'C'.", "Carla");
    inserirNaHash("Uma luva suja de tinta.", "Roberto");
    inserirNaHash("Um colar caro deixado sobre a mesa.", "Carla");
    inserirNaHash("Uma anotação sobre veneno.", "Dra. Elisa");
    inserirNaHash("Um perfume raro de uso masculino.", "Roberto");
    inserirNaHash("Uma chave com o brasão da família.", "Sr. Augusto");

    PistaNode *pistas = NULL;

    printf("=========================================\n");
    printf("   Detective Quest: Julgamento Final\n");
    printf("=========================================\n");
    printf("Você inicia sua investigação no Hall de Entrada.\n");

    explorarSalas(hall, &pistas);

    printf("\n===== PISTAS COLETADAS =====\n");
    if (pistas == NULL)
        printf("Nenhuma pista coletada!\n");
    else
        exibirPistas(pistas);

    verificarSuspeitoFinal(pistas);

    liberarSalas(hall);
    liberarPistas(pistas);
    liberarHash();

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}