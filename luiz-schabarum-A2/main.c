#include "gbv.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Uso: %s <opção> <biblioteca> [documentos...]\n", argv[0]);
        return 1;
    }

    const char *opcao = argv[1];
    const char *biblioteca = argv[2];

    char input[100];
    printf("Digite a chave: ");
    fflush(stdout);
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Erro ao ler a chave.\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        input[--len] = '\0';

    if (len != 4)
    {
        printf("A chave deve ter exatamente quatro caracteres.\n");
        return 1;
    }
    char key[5];
    strcpy(key, input);

    Library lib;
    if (gbv_open(&lib, biblioteca, key) != 0)
    {
        printf("Erro ao abrir biblioteca %s.\n", biblioteca);
        return 1;
    }

    if (strcmp(opcao, "-a") == 0)
    {
        for (int i = 3; i < argc; i++)
        {
            gbv_add(&lib, biblioteca, argv[i]);
        }
    }
    else if (strcmp(opcao, "-r") == 0)
    {
        for (int i = 3; i < argc; i++)
        {
            gbv_remove(&lib, argv[i]);
        }
    }
    else if (strcmp(opcao, "-l") == 0)
    {
        gbv_list(&lib);
    }
    else if (strcmp(opcao, "-v") == 0 && argc >= 4)
    {
        gbv_view(&lib, argv[3]);
    }
    else if (strcmp(opcao, "-o") == 0 && argc >= 4)
    {
        gbv_order(&lib, biblioteca, argv[3]);
    }
    else
    {
        printf("Opção inválida.\n");
    }

    return 0;
}
