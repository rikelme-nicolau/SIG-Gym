#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h" // <-- persistência

void telaExcluirEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                   EXCLUIR EQUIPAMENTO                               ===\n");
        printf("=========================================================================\n");
        printf("===                 NENHUM EQUIPAMENTO CADASTRADO                       ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                   EXCLUIR EQUIPAMENTO                               ===\n");
    printf("=========================================================================\n");

    // Listar apenas equipamentos ativos
    int algum_ativo = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            printf("[%s] %s (%s)\n", lista_equipamentos[i].id,
                   lista_equipamentos[i].nome,
                   lista_equipamentos[i].categoria);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        printf("=========================================================================\n");
        printf("===                 NENHUM EQUIPAMENTO ATIVO                             ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do equipamento que deseja excluir: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            lista_equipamentos[i].ativo = false; // desativa o equipamento

            // **Persistência automática**
            excluirEquipamento(id_busca);
            limparTela();
            printf("=========================================================================\n");
            printf("===                   EXCLUIR EQUIPAMENTO                               ===\n");
            printf("=========================================================================\n");
            printf("===                 EQUIPAMENTO EXCLUIDO COM SUCESSO                   ===\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                   EXCLUIR EQUIPAMENTO                               ===\n");
        printf("=========================================================================\n");
        printf("===                 EQUIPAMENTO NÃO ENCONTRADO                           ===\n");
        printf("=========================================================================\n");
    }

    getchar();
    limparTela();
}
