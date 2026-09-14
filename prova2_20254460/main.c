#include "arvoreB.h"

int
main ()
{
  // cria arvore B com grau minimo t = 3
  struct arvoreB *arvore = criarArvoreB (3);

  // preenche a arvore
  for (size_t i = 0; i < 20; i++)
    {
      inserirArvoreB (arvore, i);
    }

  // imprime as chaves em ordem
  printf ("Chaves em ordem:\n");
  imprimirEmOrdem (arvore);

  // imprime a estrutura da arvore
  printf ("\nEstrutura da arvore B antes das remoções:\n");
  imprimirArvoreB (arvore);

  // remoção das chaves 0 e 3
  printf ("\nRemover chave 0: \n");
  removerChaveArvoreB (arvore, 0);
  imprimirEmOrdem (arvore);

  printf ("Remover chave 3: ---\n");
  removerChaveArvoreB (arvore, 3);
  imprimirEmOrdem (arvore);

  // imprime a estrutura da arvore
  printf ("\nEstrutura da arvore B depois das remoções:\n");
  imprimirArvoreB (arvore);

  // teste de busca
  int32_t chaveBusca = 5;
  int32_t indice = -1;

  struct nodo *resultado = buscarArvoreB (arvore, chaveBusca, &indice);

  if (resultado != NULL)
    {
      printf ("\nChave %d encontrada no indice %d do nodo.\n", chaveBusca,
              indice);
    }
  else
    {
      printf ("\nChave %d nao encontrada.\n", chaveBusca);
    }

  // libera memoria
  deletarArvore (arvore);

  return 0;
}