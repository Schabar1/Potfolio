#include "nodo.h"

void *
alocarMemoria (size_t tamanho)
{
  void *ptr = malloc (tamanho);

  if (ptr == NULL)
    {
      fprintf (stderr, "falha ao alocar memoria\n");
      exit (1);
    }

  return ptr;
}

struct nodo *
criarNodo (int32_t t, int32_t folha)
{

  struct nodo *novo = (struct nodo *)alocarMemoria (sizeof (struct nodo));

  novo->folha = folha;
  novo->n = 0;

  novo->chaves
      = (int32_t *)alocarMemoria ((size_t)(2 * t - 1) * sizeof (int32_t));

  novo->filhos = (struct nodo **)alocarMemoria ((size_t)(2 * t)
                                                * sizeof (struct nodo *));

  return novo;
}

void
dividirFilho (struct nodo *pai, int32_t i, struct nodo *filho, int32_t t)
{

  int32_t j;

  struct nodo *novo = criarNodo (t, filho->folha);

  novo->n = t - 1;

  // copia as ultimas t-1 chaves do filho para o novo nodo
  for (j = 0; j < t - 1; j++)
    {
      novo->chaves[j] = filho->chaves[j + t];
    }

  // copia os ultimos t filhos, se nao for folha
  if (filho->folha == 0)
    {
      for (j = 0; j < t; j++)
        {
          novo->filhos[j] = filho->filhos[j + t];
        }
    }

  filho->n = t - 1;

  // move os filhos do pai para abrir espaco
  for (j = pai->n; j >= i + 1; j--)
    {
      pai->filhos[j + 1] = pai->filhos[j];
    }

  pai->filhos[i + 1] = novo;

  // move as chaves do pai
  for (j = pai->n - 1; j >= i; j--)
    {
      pai->chaves[j + 1] = pai->chaves[j];
    }

  // chave do meio sobe para o pai
  pai->chaves[i] = filho->chaves[t - 1];

  pai->n = pai->n + 1;
}

void
inserirNaoCheio (struct nodo *nodoAtual, int32_t chave, int32_t t)
{

  int32_t i = nodoAtual->n - 1;

  if (nodoAtual->folha == 1)
    {

      // move as chaves maiores para abrir espaco
      while (i >= 0 && chave < nodoAtual->chaves[i])
        {
          nodoAtual->chaves[i + 1] = nodoAtual->chaves[i];
          i = i - 1;
        }

      nodoAtual->chaves[i + 1] = chave;

      nodoAtual->n = nodoAtual->n + 1;
    }
  else
    {

      // encontra o filho que vai receber a chave
      while (i >= 0 && chave < nodoAtual->chaves[i])
        {
          i = i - 1;
        }

      i = i + 1;

      // divide o filho caso esteja cheio
      if (nodoAtual->filhos[i]->n == 2 * t - 1)
        {

          dividirFilho (nodoAtual, i, nodoAtual->filhos[i], t);

          if (chave > nodoAtual->chaves[i])
            {
              i = i + 1;
            }
        }

      inserirNaoCheio (nodoAtual->filhos[i], chave, t);
    }
}

int32_t
alturaArvore (struct nodo *nodoAtual)
{

  // se nodo for nulo retorna 0
  if (nodoAtual == NULL)
    {
      return 0;
    }

  // se nodo for folha retorna 1
  if (nodoAtual->folha == 1)
    {
      return 1;
    }

  // retorna 1+ chamada recursiva da função
  return 1 + alturaArvore (nodoAtual->filhos[0]);
}

void
imprimirNivel (struct nodo *nodoAtual, int32_t nivel)
{

  if (nodoAtual == NULL)
    {
      return;
    }

  if (nivel == 0)
    {

      if (nodoAtual->folha == 1)
        {
          printf ("F ");
        }
      else
        {
          printf ("I ");
        }

      printf ("(n:%d) [", nodoAtual->n);

      int32_t i;

      for (i = 0; i < nodoAtual->n; i++)
        {

          printf ("%d", nodoAtual->chaves[i]);

          if (i < nodoAtual->n - 1)
            {
              printf (" ");
            }
        }

      printf ("] ");

      return;
    }

  if (nodoAtual->folha == 0)
    {

      int32_t i;

      for (i = 0; i <= nodoAtual->n; i++)
        {
          imprimirNivel (nodoAtual->filhos[i], nivel - 1);
        }
    }
}

struct nodo *
buscarNodo (struct nodo *nodoAtual, int32_t chave, int32_t *idxEncontrado)
{

  int32_t i = 0;

  // encontra a primeira chave maior ou igual
  while (i < nodoAtual->n && chave > nodoAtual->chaves[i])
    {
      i = i + 1;
    }

  // se encontrou a chave
  if (i < nodoAtual->n && chave == nodoAtual->chaves[i])
    {
      if (idxEncontrado != NULL)
        {
          *idxEncontrado = i;
        }
      return nodoAtual;
    }

  // se for folha, nao encontrou
  if (nodoAtual->folha == 1)
    {
      return NULL;
    }

  // continua até achar filho
  return buscarNodo (nodoAtual->filhos[i], chave, idxEncontrado);
}

void
imprimirEmOrdemNodo (struct nodo *nodoAtual)
{

  if (nodoAtual == NULL)
    {
      return;
    }

  int32_t i;

  for (i = 0; i < nodoAtual->n; i++)
    {

      if (nodoAtual->folha == 0)
        {
          imprimirEmOrdemNodo (nodoAtual->filhos[i]);
        }

      printf ("%d ", nodoAtual->chaves[i]);
    }

  if (nodoAtual->folha == 0)
    {
      imprimirEmOrdemNodo (nodoAtual->filhos[i]);
    }
}

void
deletarNodo (struct nodo *nodoAtual)
{

  if (nodoAtual == NULL)
    {
      return;
    }

  int32_t i;

  if (nodoAtual->folha == 0)
    {
      for (i = 0; i <= nodoAtual->n; i++)
        {
          deletarNodo (nodoAtual->filhos[i]);
        }
    }

  free (nodoAtual->chaves);
  free (nodoAtual->filhos);
  free (nodoAtual);
}