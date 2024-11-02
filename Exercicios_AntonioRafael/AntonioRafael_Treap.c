#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo um tipo para simplificar o uso do NoTreap
typedef struct NoTreap {
    int chave, prioridade;
    struct NoTreap *esquerda, *direita;
} NoTreap;

// Função para criar um novo no da Treap
NoTreap* criarNo(int chave) {
    NoTreap* no = (NoTreap*)malloc(sizeof(NoTreap));
    no->chave = chave;
    no->prioridade = rand() % 100;
    no->esquerda = no->direita = NULL;
    return no;
}

// Funções de rotação
NoTreap* rotacionarDireita(NoTreap* raiz) {
    NoTreap* novaRaiz = raiz->esquerda;
    raiz->esquerda = novaRaiz->direita;
    novaRaiz->direita = raiz;
    return novaRaiz;
}

NoTreap* rotacionarEsquerda(NoTreap* raiz) {
    NoTreap* novaRaiz = raiz->direita;
    raiz->direita = novaRaiz->esquerda;
    novaRaiz->esquerda = raiz;
    return novaRaiz;
}

// Função para inserir um novo no na Treap
NoTreap* inserir(NoTreap* raiz, int chave) {
    if (raiz == NULL)
        return criarNo(chave);

    if (chave < raiz->chave) {
        raiz->esquerda = inserir(raiz->esquerda, chave);
        if (raiz->esquerda->prioridade > raiz->prioridade)
            raiz = rotacionarDireita(raiz);
    } else if (chave > raiz->chave) {
        raiz->direita = inserir(raiz->direita, chave);
        if (raiz->direita->prioridade > raiz->prioridade)
            raiz = rotacionarEsquerda(raiz);
    }
    return raiz;
}

// Função para deletar um no da Treap
NoTreap* deletarNo(NoTreap* raiz, int chave) {
    if (raiz == NULL)
        return raiz;

    if (chave < raiz->chave) {
        raiz->esquerda = deletarNo(raiz->esquerda, chave);
    } else if (chave > raiz->chave) {
        raiz->direita = deletarNo(raiz->direita, chave);
    } else {
        if (raiz->esquerda == NULL) {
            NoTreap* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            NoTreap* temp = raiz->esquerda;
            free(raiz);
            return temp;
        } else if (raiz->esquerda->prioridade < raiz->direita->prioridade) {
            raiz = rotacionarEsquerda(raiz);
            raiz->esquerda = deletarNo(raiz->esquerda, chave);
        } else {
            raiz = rotacionarDireita(raiz);
            raiz->direita = deletarNo(raiz->direita, chave);
        }
    }
    return raiz;
}

// Função de busca na Treap
NoTreap* buscar(NoTreap* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;
    if (chave < raiz->chave)
        return buscar(raiz->esquerda, chave);
    return buscar(raiz->direita, chave);
}

// Função auxiliar para impressão da Treap com indentação
void imprimirTreapAuxiliar(NoTreap* raiz, int profundidade) {
    if (raiz == NULL) {
        for (int i = 0; i < profundidade; i++) printf("    ");
        printf("~\n");
        return;
    }
    imprimirTreapAuxiliar(raiz->direita, profundidade + 1);

    for (int i = 0; i < profundidade; i++) printf("    ");
    printf("%d (Prioridade = %d)\n", raiz->chave, raiz->prioridade);

    imprimirTreapAuxiliar(raiz->esquerda, profundidade + 1);
}

// Função principal de impressão da Treap
void imprimirTreap(NoTreap* raiz) {
    printf("Estrutura da Treap:\n");
    imprimirTreapAuxiliar(raiz, 0);
}

// Função para liberar memória da Treap
void destruirTreap(NoTreap* raiz) {
    if (raiz != NULL) {
        destruirTreap(raiz->esquerda);
        destruirTreap(raiz->direita);
        free(raiz);
    }
}

// Função principal
int main() {
    srand(time(NULL));
    NoTreap* raiz = NULL;

    // Inserção de nos na Treap
    raiz = inserir(raiz, 50);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 40);
    raiz = inserir(raiz, 70);
    raiz = inserir(raiz, 60);
    raiz = inserir(raiz, 80);

    // Imprime a Treap
    imprimirTreap(raiz);

    printf("\nDeletando o no 20\n");
    raiz = deletarNo(raiz, 20);
    imprimirTreap(raiz);

    printf("\nDeletando o no 30\n");
    raiz = deletarNo(raiz, 30);
    imprimirTreap(raiz);

    printf("\nDeletando o no 50\n");
    raiz = deletarNo(raiz, 50);
    imprimirTreap(raiz);

    int valorbusc = 60;
    NoTreap* res = buscar(raiz, valorbusc);
    if (res)
        printf("\n%d encontrado na Treap\n", valorbusc);
    else
        printf("\n%d não encontrado na Treap\n", valorbusc);

    // Libera a memória da Treap
    destruirTreap(raiz);

    return 0;
}
