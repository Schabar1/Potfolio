#ifndef NODO
#define NODO

#include "arvoreB.h"

// função para tratar erro de alocação de memória
void *alocarMemoria (size_t tamanho);

// função que cria novo nodo
struct nodo *criarNodo (int32_t t, int32_t folha);

// função que divide o filho cheio de um nodo
void dividirFilho (struct nodo *pai, int32_t i, struct nodo *filho, int32_t t);

// função para inserir uma chave em um nodo não cheio
void inserirNaoCheio (struct nodo *nodoAtual, int32_t chave, int32_t t);

// função que calcula altura do nodo atual
int32_t alturaArvore (struct nodo *nodoAtual);

// função para imprimir nodos de um nivel especifico
void imprimirNivel (struct nodo *nodoAtual, int32_t nivel);

// função recursiva de busca de chave em nodos na arvore B
struct nodo *buscarNodo (struct nodo *nodoAtual, int32_t chave,
                         int32_t *idxEncontrado);

// função para imprimir as chaves da arvore em ordem
void imprimirEmOrdemNodo (struct nodo *nodoAtual);

// função para deletar arvore B nodo a nodo
void deletarNodo (struct nodo *nodoAtual);

bool removerChaveNodo (struct arvoreB *arvore, struct nodo *nodoAtual,
                       int32_t chave);
void mergeNodos (struct nodo *pai, int32_t idx, int32_t t);

void emprestarEsquerda (struct nodo *pai, int32_t idx);

void emprestarDireita (struct nodo *pai, int32_t idx);

int32_t encontrarPredecessor (struct nodo *nodo);

int32_t encontrarSucessor (struct nodo *nodo);

#endif