#include <stdio.h>
#include <stdlib.h>

#define MIN_DEGREE 3
#define MAX_DEGREE 7

// Estrutura de um nó da B-tree
struct BTreeNode {
    int *chaves;
    int num_chaves;
    struct BTreeNode **filhos;
    int grau;
    int folha;
};

// Função para criar um novo nó da B-tree
struct BTreeNode* criarNo(int grau, int folha) {
    struct BTreeNode* novo_no = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    novo_no->grau = grau;
    novo_no->folha = folha;
    novo_no->chaves = (int*)malloc((2 * grau - 1) * sizeof(int));
    novo_no->filhos = (struct BTreeNode**)malloc(2 * grau * sizeof(struct BTreeNode*));
    novo_no->num_chaves = 0;
    return novo_no;
}

// Função auxiliar para dividir um filho cheio
void dividirFilho(struct BTreeNode *pai, int i) {
    int grau = pai->grau;
    struct BTreeNode *filho = pai->filhos[i];
    struct BTreeNode *novo_no = criarNo(grau, filho->folha);
    novo_no->num_chaves = grau - 1;

    // Move as últimas chaves do filho para o novo nó
    for (int j = 0; j < grau - 1; j++) {
        novo_no->chaves[j] = filho->chaves[j + grau];
    }
    if (!filho->folha) {
        for (int j = 0; j < grau; j++) {
            novo_no->filhos[j] = filho->filhos[j + grau];
        }
    }
    filho->num_chaves = grau - 1;

    // Move os filhos do pai para abrir espaço para o novo nó
    for (int j = pai->num_chaves; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[i + 1] = novo_no;

    // Move as chaves do pai
    for (int j = pai->num_chaves - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }
    pai->chaves[i] = filho->chaves[grau - 1];
    pai->num_chaves++;
}

// Função para inserir em um nó não cheio
void inserirNaoCheio(struct BTreeNode *no, int chave) {
    int i = no->num_chaves - 1;

    if (no->folha) {
        while (i >= 0 && no->chaves[i] > chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->num_chaves++;
    } else {
        while (i >= 0 && no->chaves[i] > chave) {
            i--;
        }
        i++;
        if (no->filhos[i]->num_chaves == 2 * no->grau - 1) {
            dividirFilho(no, i);
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

// Função para inserir uma chave na B-tree
void inserir(struct BTreeNode **raiz, int chave) {
    struct BTreeNode *r = *raiz;

    if (r->num_chaves == 2 * r->grau - 1) {
        struct BTreeNode *novo_no = criarNo(r->grau, 0);
        novo_no->filhos[0] = r;
        dividirFilho(novo_no, 0);
        int i = 0;
        if (novo_no->chaves[0] < chave) {
            i++;
        }
        inserirNaoCheio(novo_no->filhos[i], chave);
        *raiz = novo_no;
    } else {
        inserirNaoCheio(r, chave);
    }
}

// Função para buscar uma chave na B-tree
struct BTreeNode* buscar(struct BTreeNode* no, int chave) {
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i]) {
        i++;
    }
    if (i < no->num_chaves && no->chaves[i] == chave) {
        return no;  // Chave encontrada
    }
    if (no->folha) {
        return NULL;  // Chave não encontrada
    }
    return buscar(no->filhos[i], chave);
}

// Função para imprimir a B-tree em ordem
void imprimirEmOrdem(struct BTreeNode* no) {
    if (no != NULL) {
        for (int i = 0; i < no->num_chaves; i++) {
            // Imprimir os filhos antes da chave
            if (!no->folha) {
                imprimirEmOrdem(no->filhos[i]);
            }
            printf("%d ", no->chaves[i]);  // Imprimir a chave
        }
        // Imprimir o último filho
        if (!no->folha) {
            imprimirEmOrdem(no->filhos[no->num_chaves]);
        }
    }
}

// Função para liberar a memória
void liberarBTree(struct BTreeNode* no) {
    if (no != NULL) {
        if (!no->folha) {
            for (int i = 0; i <= no->num_chaves; i++) {
                liberarBTree(no->filhos[i]);
            }
        }
        free(no->chaves);
        free(no->filhos);
        free(no);
    }
}

// Função principal
int main() {
    int grau = MIN_DEGREE;
    struct BTreeNode* raiz = criarNo(grau, 1);

    // Inserindo algumas chaves para teste
    inserir(&raiz, 10);
    inserir(&raiz, 20);
    inserir(&raiz, 5);
    inserir(&raiz, 6);
    inserir(&raiz, 12);
    inserir(&raiz, 30);
    inserir(&raiz, 25);  // Adicionando mais uma chave para ver melhor a estrutura

    printf("Impressao da B-tree em ordem:\n");
    imprimirEmOrdem(raiz);
    printf("\n");

    // Teste de busca
    int chave_busca = 12;
    struct BTreeNode* resultado = buscar(raiz, chave_busca);
    if (resultado) {
        printf("Chave %d encontrada na B-tree.\n", chave_busca);
    } else {
        printf("Chave %d não encontrada na B-tree.\n", chave_busca);
    }

    // Liberar memória
    liberarBTree(raiz);
    return 0;
}
