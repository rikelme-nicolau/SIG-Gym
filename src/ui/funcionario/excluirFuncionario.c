#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h" // <-- persistência

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
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
    printf("=========================================================================\n");

    // Listar apenas funcionários ativos
    int algum_ativo = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            printf("[%s] %s (%s)\n", lista_funcionarios[i].id,
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
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do funcionário que deseja excluir: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            lista_funcionarios[i].ativo = false; // desativa o funcionário

            // **Persistência automática**
            excluirFuncionario(id_busca);
            limparTela();
            printf("=========================================================================\n");
            printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
            printf("=========================================================================\n");
            printf("===                 FUNCIONÁRIO EXCLUIDO COM SUCESSO                   ===\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                    EXCLUIR FUNCIONÁRIO                             ===\n");
        printf("=========================================================================\n");
        printf("===                 FUNCIONÁRIO NÃO ENCONTRADO                         ===\n");
        printf("=========================================================================\n");
    }

    getchar();
    limparTela();
}
