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

// funde o filho na posição idx e idx+1, usando a chave pai->chaves[idx]
void
mergeNodos (struct nodo *pai, int32_t idx, int32_t t)
{
  struct nodo *esquerdo = pai->filhos[idx];
  struct nodo *direito = pai->filhos[idx + 1];

  // move a chave do pai para o nó esquerdo
  esquerdo->chaves[t - 1] = pai->chaves[idx];

  // copia chaves do nodo direito
  for (int32_t j = 0; j < direito->n; j++)
    esquerdo->chaves[t + j] = direito->chaves[j];

  // copia filhos do nodo direito
  if (!esquerdo->folha)
    {
      for (int32_t j = 0; j <= direito->n; j++)
        esquerdo->filhos[t + j] = direito->filhos[j];
    }

  esquerdo->n = 2 * t - 1;

  // remove a chave do pai e desloca os filhos
  for (int32_t j = idx; j < pai->n - 1; j++)
    pai->chaves[j] = pai->chaves[j + 1];

  for (int32_t j = idx + 1; j < pai->n; j++)
    pai->filhos[j] = pai->filhos[j + 1];

  pai->n--;

  // libera o nodo
  free (direito->chaves);
  free (direito->filhos);
  free (direito);
}

// pega uma chave emprestada do irmão esquerdo
void
emprestarEsquerda (struct nodo *pai, int32_t idx)
{
  struct nodo *esquerdo = pai->filhos[idx - 1];
  struct nodo *atual = pai->filhos[idx];

  // desloca as chaves do atual para a direita
  for (int32_t j = atual->n - 1; j >= 0; j--)
    atual->chaves[j + 1] = atual->chaves[j];

  // se não for folha, desloca os filhos
  if (!atual->folha)
    {
      for (int32_t j = atual->n; j >= 0; j--)
        atual->filhos[j + 1] = atual->filhos[j];
    }

  // a chave do pai desce para o atual
  atual->chaves[0] = pai->chaves[idx - 1];

  // o filho mais da direita do nodo esquerdo sobe para o pai
  pai->chaves[idx - 1] = esquerdo->chaves[esquerdo->n - 1];

  if (!atual->folha)
    atual->filhos[0] = esquerdo->filhos[esquerdo->n];

  esquerdo->n--;
  atual->n++;
}

// pega uma chave emprestada do irmão direito
void
emprestarDireita (struct nodo *pai, int32_t idx)
{
  struct nodo *atual = pai->filhos[idx];
  struct nodo *direito = pai->filhos[idx + 1];

  // a chave do pai desce para o atual
  atual->chaves[atual->n] = pai->chaves[idx];

  // o filho mais da esquerda do nodo direito sobe para o pai
  pai->chaves[idx] = direito->chaves[0];

  // desloca as chaves do direito para a esquerda
  for (int32_t j = 1; j < direito->n; j++)
    direito->chaves[j - 1] = direito->chaves[j];

  // se não folha, desloca os filhos
  if (!atual->folha)
    {
      atual->filhos[atual->n + 1] = direito->filhos[0];
      for (int32_t j = 1; j <= direito->n; j++)
        direito->filhos[j - 1] = direito->filhos[j];
    }

  direito->n--;
  atual->n++;
}

// retorna a maior chave da sub-árvore
int32_t
encontrarPredecessor (struct nodo *nodo)
{
  while (!nodo->folha)
    nodo = nodo->filhos[nodo->n];
  return nodo->chaves[nodo->n - 1];
}

// retorna a menor chave da sub-árvore
int32_t
encontrarSucessor (struct nodo *nodo)
{
  while (!nodo->folha)
    nodo = nodo->filhos[0];
  return nodo->chaves[0];
}

// função recursiva de remoção
bool
removerChaveNodo (struct arvoreB *arvore, struct nodo *nodoAtual,
                  int32_t chave)
{
  int32_t t = arvore->t_arvore;
  int32_t i = 0;

  // encontra a primeira posição onde chave <= nodoAtual->chaves[i]
  while (i < nodoAtual->n && chave > nodoAtual->chaves[i])
    i++;

  // 1: a chave está neste nó
  if (i < nodoAtual->n && chave == nodoAtual->chaves[i])
    {
      // 1a: nó folha -> remove diretamente
      if (nodoAtual->folha)
        {
          for (int32_t j = i; j < nodoAtual->n - 1; j++)
            nodoAtual->chaves[j] = nodoAtual->chaves[j + 1];
          nodoAtual->n--;
          return true;
        }
      // 1b: nó interno
      else
        {
          struct nodo *filhoEsq = nodoAtual->filhos[i];
          struct nodo *filhoDir = nodoAtual->filhos[i + 1];

          // se o filho esquerdo tem pelo menos t chaves, usa o predecessor
          if (filhoEsq->n >= t)
            {
              int32_t pred = encontrarPredecessor (filhoEsq);
              nodoAtual->chaves[i] = pred;
              return removerChaveNodo (arvore, filhoEsq, pred);
            }
          // se o filho direito tem pelo menos t chaves, usa o sucessor
          else if (filhoDir->n >= t)
            {
              int32_t succ = encontrarSucessor (filhoDir);
              nodoAtual->chaves[i] = succ;
              return removerChaveNodo (arvore, filhoDir, succ);
            }
          // ambos têm t-1 chaves -> funde os dois filhos com a chave atual
          else
            {
              mergeNodos (nodoAtual, i, t);
              // a chave foi movida para o nó fundido
              return removerChaveNodo (arvore, filhoEsq, chave);
            }
        }
    }
  // 2: a chave não está neste nó (tá em algum filho)
  else
    {
      // se o nó é folha, a chave não existe
      if (nodoAtual->folha)
        return false;

      bool precisaCorrigir = (nodoAtual->filhos[i]->n == t - 1);
      struct nodo *filhoAlvo = nodoAtual->filhos[i];

      // se o filho alvo tem t-1 chaves, precisa ter pelo menos t
      if (precisaCorrigir)
        {
          // tenta emprestar do irmão esquerdo
          if (i > 0 && nodoAtual->filhos[i - 1]->n >= t)
            {
              emprestarEsquerda (nodoAtual, i);
            }
          // tenta emprestar do irmão direito
          else if (i < nodoAtual->n && nodoAtual->filhos[i + 1]->n >= t)
            {
              emprestarDireita (nodoAtual, i);
            }
          // senão, funde com um irmão
          else
            {
              if (i < nodoAtual->n) // funde com o direito
                {
                  mergeNodos (nodoAtual, i, t);
                  filhoAlvo
                      = nodoAtual->filhos[i]; // o fundidofica na posição i
                }
              else if (i > 0) // funde com o esquerdo
                {
                  mergeNodos (nodoAtual, i - 1, t);
                  filhoAlvo = nodoAtual->filhos[i - 1];
                }
            }
        }

      // desce recursivamente
      return removerChaveNodo (arvore, filhoAlvo, chave);
    }
}