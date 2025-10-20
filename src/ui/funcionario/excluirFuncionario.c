#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"

// tirar \r e \n
void limparString(char *str)
{
    str[strcspn(str, "\r\n")] = '\0';
    for (int i = strlen(str) - 1; i >= 0 && str[i] == ' '; i--)
        str[i] = '\0';
}

// func reutilizada pro problema de uffers
void lerString(char *dest, int tamanho)
{
    fgets(dest, tamanho, stdin);
    limparString(dest);
}

void telaExcluirFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
        printf("=========================================================================\n");
        printf("===                  NENHUM FUNCIONÁRIO CADASTRADO                     ===\n");
        printf("=========================================================================\n");
        printf("Pressione ENTER para voltar...");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
    printf("=========================================================================\n");

    int algum_ativo = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            printf("[%s] %s (%s)\n",
                   lista_funcionarios[i].id,
                   lista_funcionarios[i].nome,
                   lista_funcionarios[i].cargo);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        printf("=========================================================================\n");
        printf("===                  NENHUM FUNCIONÁRIO ATIVO                           ===\n");
        printf("=========================================================================\n");
        printf("Pressione ENTER para voltar...");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do funcionário que deseja excluir: ");
    char id_busca[32];
    lerString(id_busca, sizeof(id_busca));

    int encontrado = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        char id_func[32];
        strcpy(id_func, lista_funcionarios[i].id);
        limparString(id_func);

        if (strcmp(id_func, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            lista_funcionarios[i].ativo = false;
            salvarFuncionarios(lista_funcionarios, total_funcionarios);

            limparTela();
            printf("=========================================================================\n");
            printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
            printf("=========================================================================\n");
            printf("===                 FUNCIONÁRIO EXCLUÍDO COM SUCESSO                   ===\n");
            printf("=========================================================================\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            limparTela();
            return;
        }
    }

    printf("=========================================================================\n");
    printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
    printf("=========================================================================\n");
    printf("===                 FUNCIONÁRIO NÃO ENCONTRADO                         ===\n");
    printf("=========================================================================\n");
    printf("Pressione ENTER para voltar...");
    getchar();
    limparTela();
}
