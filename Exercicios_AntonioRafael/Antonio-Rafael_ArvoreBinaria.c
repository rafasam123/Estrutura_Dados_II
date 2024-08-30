#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da árvore binária
typedef struct No{
  int dados;
  struct No *direita;
  struct No *esquerda;
}No;

// Função para criar um novo nó
No *criarNo(int dados){
  No *novoNo = (No*)malloc(sizeof(No));
  if (novoNo == NULL)
    {
        printf("Erro: Falha ao alocar memória para o novo nó.\n");
        exit(-1);
    }
  novoNo->dados = dados;
  novoNo->direita = NULL;
  novoNo->esquerda = NULL;
  return novoNo;
}

// Função para inserir um nó na árvore binária
No *inserirNo(No *raiz, int dados){
  if (raiz == NULL){
    raiz = criarNo(dados);
  }
  else{
    if (dados <= raiz->dados){
      raiz->esquerda = inserirNo(raiz->esquerda, dados);
    }
    else{
      raiz->direita = inserirNo(raiz->direita, dados);
    }
  }
  return raiz;
}

// Função para encontrar o nó com o menor valor em uma árvore
No *minimoNo(No *raiz) {
    while (raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

// Função para excluir um nó da árvore binária
No *excluirNo(No *raiz, int dados){
  if (raiz == NULL){
    return raiz;
  }
  if (dados < raiz->dados){
    raiz->esquerda = excluirNo(raiz->esquerda, dados);
  }
  else if(dados > raiz->dados){
    raiz->direita = excluirNo(raiz->direita, dados);
  }
  else
  {
      // Caso 1: Nó folha ou nó com apenas um filho
      if (raiz->esquerda == NULL)
      {
        No *temp = raiz->direita;
        free(raiz);
        return temp;
      }
      else if (raiz->direita == NULL)
      {
        No *temp = raiz->esquerda;
        free(raiz);
        return temp;
      }
    // Caso 2: Nó com dois filhos
    No *temp = minimoNo(raiz->direita);

    // Copia o conteúdo do menor valor para o nó a ser deletado
    raiz->dados = temp->dados;

    // Deleta o menor valor no subárvore à direita
    raiz->direita = excluirNo(raiz->direita, temp->dados);
  }
  return raiz;
}

// Função para pesquisar um elemento na árvore binária
No *procuraNo(No *raiz, int dados) {
    if (raiz == NULL || raiz->dados == dados) {
        return raiz;
    }

    if (dados < raiz->dados) {
        return procuraNo(raiz->esquerda, dados);
    }

    return procuraNo(raiz->direita, dados);
}

// Função para imprimir a árvore binária
void imprimeArvore(No *raiz, int level) {
    if (raiz == NULL) {
        return;
    }

    // Imprime os filhos à direita
    imprimeArvore(raiz->direita, level + 1);

    // Imprime o nó atual
    for (int i = 0; i < level; i++) {
        printf("    "); // Espaçamento para cada nível
    }
    printf("%d\n", raiz->dados);

    // Imprime os filhos à esquerda
    imprimeArvore(raiz->esquerda, level + 1);
}

int main() {
    No *raiz = NULL;
    raiz = inserirNo(raiz, 50);
    inserirNo(raiz, 30);
    inserirNo(raiz, 20);
    inserirNo(raiz, 40);
    inserirNo(raiz, 70);
    inserirNo(raiz, 60);
    inserirNo(raiz, 80);
    inserirNo(raiz, 10);

    printf("Arvore Montada:\n");
    imprimeArvore(raiz, 0);
  
    int valorParaExcluir = 20;
    if (procuraNo(raiz, valorParaExcluir) != NULL) {
      printf("\nDeletar valor: %d\n", valorParaExcluir);
      raiz = excluirNo(raiz, valorParaExcluir);
    }
    else {
      printf("\nValor %d não encontrado na árvore.\n", valorParaExcluir);
    }
    printf("Arvore Montada:\n");
    imprimeArvore(raiz, 0);

    printf("\nDeletando 30\n");
    raiz = excluirNo(raiz, 30);
    printf("Arvore Montada:\n");
    imprimeArvore(raiz, 0);

    printf("\nDeletando 50\n");
    raiz = excluirNo(raiz, 50);
    printf("Arvore Montada:\n");
    imprimeArvore(raiz, 0);

    valorParaExcluir = 30;
    if (procuraNo(raiz, valorParaExcluir) != NULL) {
      printf("\nDeletar valor: %d\n", valorParaExcluir);
      raiz = excluirNo(raiz, valorParaExcluir);
    }
    else {
      printf("\nValor %d não encontrado na árvore.\n", valorParaExcluir);
    }

    return 0;
}
