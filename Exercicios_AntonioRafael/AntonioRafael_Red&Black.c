#include <stdio.h>
#include <stdlib.h>

// Definição dos possíveis valores de cor
#define VERMELHO 0
#define PRETO 1

// Definição da estrutura de um nó da árvore Red-Black
typedef struct No {
    int valor;
    int cor;
    struct No *esquerda, *direita, *pai;
} No;

// Função para criar um novo nó
No *criarNo(int valor) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->cor = VERMELHO;
    novoNo->esquerda = novoNo->direita = novoNo->pai = NULL;
    return novoNo;
}

// Função auxiliar para verificar se um nó é vermelho
int ehVermelho(No *no) {
    return no != NULL && no->cor == VERMELHO;
}

// Função auxiliar para definir a cor de um nó
void setCor(No *no, int cor) {
    if (no != NULL) {
        no->cor = cor;
    }
}

// Função para fazer a rotação à esquerda
void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL)
        y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

// Função para fazer a rotação à direita
void rotacaoDireita(No **raiz, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != NULL)
        y->direita->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->direita)
        x->pai->direita = y;
    else
        x->pai->esquerda = y;
    y->direita = x;
    x->pai = y;
}

// Função para balancear a árvore após a inserção
void corrigirViolacao(No **raiz, No *z) {
    while (ehVermelho(z->pai)) {
        if (z->pai == z->pai->pai->esquerda) {
            No *y = z->pai->pai->direita;
            if (ehVermelho(y)) {
                setCor(z->pai, PRETO);
                setCor(y, PRETO);
                setCor(z->pai->pai, VERMELHO);
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                setCor(z->pai, PRETO);
                setCor(z->pai->pai, VERMELHO);
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            No *y = z->pai->pai->esquerda;
            if (ehVermelho(y)) {
                setCor(z->pai, PRETO);
                setCor(y, PRETO);
                setCor(z->pai->pai, VERMELHO);
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                setCor(z->pai, PRETO);
                setCor(z->pai->pai, VERMELHO);
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    setCor(*raiz, PRETO);
}

// Função para inserir um novo nó na árvore Red-Black
void inserir(No **raiz, int valor) {
    No *z = criarNo(valor);
    No *y = NULL;
    No *x = *raiz;

    // Inserção básica de árvore binária de busca
    while (x != NULL) {
        y = x;
        if (z->valor < x->valor)
            x = x->esquerda;
        else
            x = x->direita;
    }
    z->pai = y;
    if (y == NULL)
        *raiz = z;
    else if (z->valor < y->valor)
        y->esquerda = z;
    else
        y->direita = z;

    // Corrige as propriedades da árvore Red-Black
    corrigirViolacao(raiz, z);
}

// Função para substituir um nó por outro
void substituirNo(No **raiz, No *u, No *v) {
    if (u->pai == NULL) {
        *raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    if (v != NULL) {
        v->pai = u->pai;
    }
}

// Função auxiliar para encontrar o nó mínimo (mais à esquerda)
No *minimo(No *no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

// Função para corrigir a árvore após exclusão
void corrigirExclusao(No **raiz, No *x) {
    while (x != *raiz && (x == NULL || x->cor == PRETO)) {
        if (x == x->pai->esquerda) {
            No *w = x->pai->direita;
            if (ehVermelho(w)) {
                setCor(w, PRETO);
                setCor(x->pai, VERMELHO);
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->direita;
            }
            if ((!ehVermelho(w->esquerda)) && (!ehVermelho(w->direita))) {
                setCor(w, VERMELHO);
                x = x->pai;
            } else {
                if (!ehVermelho(w->direita)) {
                    setCor(w->esquerda, PRETO);
                    setCor(w, VERMELHO);
                    rotacaoDireita(raiz, w);
                    w = x->pai->direita;
                }
                setCor(w, x->pai->cor);
                setCor(x->pai, PRETO);
                setCor(w->direita, PRETO);
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        } else {
            No *w = x->pai->esquerda;
            if (ehVermelho(w)) {
                setCor(w, PRETO);
                setCor(x->pai, VERMELHO);
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esquerda;
            }
            if ((!ehVermelho(w->direita)) && (!ehVermelho(w->esquerda))) {
                setCor(w, VERMELHO);
                x = x->pai;
            } else {
                if (!ehVermelho(w->esquerda)) {
                    setCor(w->direita, PRETO);
                    setCor(w, VERMELHO);
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esquerda;
                }
                setCor(w, x->pai->cor);
                setCor(x->pai, PRETO);
                setCor(w->esquerda, PRETO);
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    setCor(x, PRETO);
}

// Função para excluir um nó da árvore Red-Black
void excluir(No **raiz, int valor) {
    No *z = *raiz;
    while (z != NULL && z->valor != valor) {
        if (valor < z->valor) {
            z = z->esquerda;
        } else {
            z = z->direita;
        }
    }
    if (z == NULL) return;

    No *y = z;
    int yCorOriginal = y->cor;
    No *x;

    if (z->esquerda == NULL) {
        x = z->direita;
        substituirNo(raiz, z, z->direita);
    } else if (z->direita == NULL) {
        x = z->esquerda;
        substituirNo(raiz, z, z->esquerda);
    } else {
        y = minimo(z->direita);
        yCorOriginal = y->cor;
        x = y->direita;
        if (y->pai == z) {
            if (x != NULL) x->pai = y;
        } else {
            substituirNo(raiz, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        substituirNo(raiz, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        setCor(y, z->cor);
    }

    free(z);

    if (yCorOriginal == PRETO) {
        corrigirExclusao(raiz, x);
    }
}

// Função para imprimir a árvore de acordo com o formato esquerda-raiz-direita
void imprimeArvoreRB(No *raiz, int espaco) {
    if (raiz != NULL) {
        // Aumenta a distância entre os níveis
        espaco += 10;

        // Processa a subárvore direita primeiro
        imprimeArvoreRB(raiz->direita, espaco);

        // Imprime o nó atual após o espaço apropriado
        printf("\n");
        for (int i = 10; i < espaco; i++)
            printf(" ");
        if (raiz->cor == VERMELHO)
            printf("\033[31m%d\033[0m\n", raiz->valor);  // Vermelho
        else
            printf("%d\n", raiz->valor);  // Preto

        // Processa a subárvore esquerda
        imprimeArvoreRB(raiz->esquerda, espaco);
    }
}

int main()
{
    No *raiz = NULL;
    // Exemplo de inserção de valores na árvore Red-Black
    int vetor[] = {12, 31, 20, 17, 11, 8, 3, 24, 15, 33};
    int i, tam = sizeof(vetor) / sizeof(vetor[0]);
    for (i = 0; i < tam; i++)
        inserir(&raiz, vetor[i]);
    printf("Árvore Red-Black: \n");
    imprimeArvoreRB(raiz, 3);
    printf("Excluiondo a raiz 20\nÁrvore Red-Black: \n");
    excluir(&raiz, 20);
    imprimeArvoreRB(raiz, 3);
    printf("\n");

    return 0;
}
