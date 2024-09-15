#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore AVL
typedef struct NoAVL {
    int dado;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
    int altura;
} NoAVL;

// Função para criar um novo nó na árvore
NoAVL *criarNo(int dado) {
    NoAVL *novoNo = (NoAVL *)malloc(sizeof(NoAVL));
    if (!novoNo) {
        printf("Erro: Falha ao alocar memória para o novo nó.\n");
        exit(-1);
    }
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 0;
    return novoNo;
}

// Função para calcular a altura de um nó
int altura(NoAVL *no) {
    return no ? no->altura : -1;
}

// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamento(NoAVL *no) {
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

// Função para atualizar a altura de um nó
void atualizarAltura(NoAVL *no) {
    if (no) {
        int alturaEsq = altura(no->esquerda);
        int alturaDir = altura(no->direita);
        no->altura = (alturaEsq > alturaDir ? alturaEsq : alturaDir) + 1;
    }
}

// Rotação à direita
NoAVL *rotacaoDireita(NoAVL *no) {
    NoAVL *novaRaiz = no->esquerda;
    no->esquerda = novaRaiz->direita;
    novaRaiz->direita = no;

    atualizarAltura(no);
    atualizarAltura(novaRaiz);

    return novaRaiz;
}

// Rotação à esquerda
NoAVL *rotacaoEsquerda(NoAVL *no) {
    NoAVL *novaRaiz = no->direita;
    no->direita = novaRaiz->esquerda;
    novaRaiz->esquerda = no;

    atualizarAltura(no);
    atualizarAltura(novaRaiz);

    return novaRaiz;
}

// Balanceamento da árvore AVL
NoAVL *balanceamento(NoAVL *raiz, int dado) {
    atualizarAltura(raiz);
    int balanceamento = fatorBalanceamento(raiz);

    if (balanceamento > 1) {
        if (dado < raiz->esquerda->dado) {
            return rotacaoDireita(raiz);
        } else {
            raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
            return rotacaoDireita(raiz);
        }
    }

    if (balanceamento < -1) {
        if (dado > raiz->direita->dado) {
            return rotacaoEsquerda(raiz);
        } else {
            raiz->direita = rotacaoDireita(raiz->direita);
            return rotacaoEsquerda(raiz);
        }
    }

    return raiz;
}

// Função para inserir um novo nó na árvore AVL
NoAVL *inserir(NoAVL *raiz, int dado) {
    if (!raiz) {
        return criarNo(dado);
    }

    if (dado < raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = inserir(raiz->direita, dado);
    } else {
        return raiz; // Dados iguais não são permitidos
    }

    return balanceamento(raiz, dado);
}

// Função para encontrar o nó com o menor valor
NoAVL *encontrarMinimo(NoAVL *no) {
    while (no->esquerda) {
        no = no->esquerda;
    }
    return no;
}

// Função para excluir um nó na árvore AVL
NoAVL *excluir(NoAVL *raiz, int valor) {
    if (!raiz) {
        return raiz;
    }

    if (valor < raiz->dado) {
        raiz->esquerda = excluir(raiz->esquerda, valor);
    } else if (valor > raiz->dado) {
        raiz->direita = excluir(raiz->direita, valor);
    } else {
        if (!raiz->esquerda) {
            NoAVL *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            NoAVL *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        NoAVL *temp = encontrarMinimo(raiz->direita);
        raiz->dado = temp->dado;
        raiz->direita = excluir(raiz->direita, temp->dado);
    }

    return balanceamento(raiz, valor);
}

// Função auxiliar
void imprimeNo(int c, int b) {
    for (int i = 0; i < b; i++) {
        printf("   ");
    }
    printf("%i\n", c);
}


void mostraArvore(NoAVL *a, int b) {
    if (a) {
        mostraArvore(a->direita, b + 1);
        imprimeNo(a->dado, b);
        mostraArvore(a->esquerda, b + 1);
    }
}

// Função buscar um elemento na árvore
NoAVL *buscarNo(NoAVL *raiz, int valor) {
    if (!raiz || raiz->dado == valor) {
        return raiz;
    }
    return valor < raiz->dado ? buscarNo(raiz->esquerda, valor) : buscarNo(raiz->direita, valor);
}

int main() {
    NoAVL *raiz = NULL;
    // Inserindo elementos na árvore AVL
    int elementos[] = {30, 24, 20, 35, 27, 33, 38, 25, 22, 34, 40, 29};
    for (int i = 0; i < sizeof(elementos)/sizeof(elementos[0]); i++) {
        raiz = inserir(raiz, elementos[i]);
    }

    printf("Árvore AVL:\n");
    mostraArvore(raiz, 3);

    int insercoes[] = {31, 15, 23};
    for (int i = 0; i < sizeof(insercoes)/sizeof(insercoes[0]); i++) {
        printf("\nLetra %c - Insere %d ---------------------------\n\n", 'A' + i, insercoes[i]);
        raiz = inserir(raiz, insercoes[i]);
        mostraArvore(raiz, 3);
    }

    printf("\nLetra D - Remove 40 ---------------------------\n\n");
    raiz = excluir(raiz, 40);
    mostraArvore(raiz, 3);

    printf("\nLetra E - Remove 30 ---------------------------\n\n");
    raiz = excluir(raiz, 30);
    mostraArvore(raiz, 3);

    return 0;
}
