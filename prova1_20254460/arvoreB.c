#include "arvoreB.h"
#include "nodo.h"

struct arvoreB *
criarArvoreB (int32_t t_arvore)
{

  struct arvoreB *arvore
      = (struct arvoreB *)alocarMemoria (sizeof (struct arvoreB));

  arvore->t_arvore = t_arvore;

  // cria raiz como folha vazia
  arvore->raiz = criarNodo (t_arvore, 1);

  return arvore;
}

void
inserirArvoreB (struct arvoreB *arvore, int32_t chave)
{

  struct nodo *raiz = arvore->raiz;
  int32_t t = arvore->t_arvore;

  // se a raiz estiver cheia, a arvore cresce em altura
  if (raiz->n == 2 * t - 1)
    {

      struct nodo *novaRaiz = criarNodo (t, 0);

      arvore->raiz = novaRaiz;

      novaRaiz->filhos[0] = raiz;

      dividirFilho (novaRaiz, 0, raiz, t);

      inserirNaoCheio (novaRaiz, chave, t);
    }
  else
    {

      inserirNaoCheio (raiz, chave, t);
    }
}

void
imprimirArvoreB (struct arvoreB *arvore)
{

  if (arvore == NULL || arvore->raiz == NULL)
    {
      return;
    }

  int32_t h = alturaArvore (arvore->raiz);

  int32_t nivel;

  for (nivel = 0; nivel < h; nivel++)
    {

      printf ("----//----\n");
      printf ("Nivel %d\n", nivel);
      printf ("----//----\n");

      imprimirNivel (arvore->raiz, nivel);

      printf ("\n");
    }
}

void
imprimirEmOrdem (struct arvoreB *arvore)
{

  if (arvore == NULL || arvore->raiz == NULL)
    {
      return;
    }

  imprimirEmOrdemNodo (arvore->raiz);

  printf ("\n");
}

struct nodo *
buscarArvoreB (struct arvoreB *arvore, int32_t chave, int32_t *idxEncontrado)
{

  if (arvore == NULL || arvore->raiz == NULL)
    {
      return NULL;
    }

  return buscarNodo (arvore->raiz, chave, idxEncontrado);
}

void
deletarArvore (struct arvoreB *arvore)
{

  if (arvore == NULL)
    {
      return;
    }

  deletarNodo (arvore->raiz);

  free (arvore);
}