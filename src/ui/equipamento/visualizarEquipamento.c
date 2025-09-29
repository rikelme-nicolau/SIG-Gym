#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"

void telaVisualizarEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                  VISUALIZAR EQUIPAMENTOS                            ===\n");
        printf("=========================================================================\n");
        printf("===                 NENHUM EQUIPAMENTO CADASTRADO                       ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                  VISUALIZAR EQUIPAMENTOS                            ===\n");
    printf("=========================================================================\n");

    // Listar apenas equipamentos ativos
    int algum_ativo = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            printf("[%s] %s\n", lista_equipamentos[i].id, lista_equipamentos[i].nome);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        printf("=========================================================================\n");
        printf("===                 NENHUM EQUIPAMENTO ATIVO                            ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do equipamento que deseja visualizar: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                  INFORMAÇÕES DO EQUIPAMENTO                         ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_equipamentos[i].id);
            printf("Nome: %s\n", lista_equipamentos[i].nome);
            printf("Última manutenção: %s\n", lista_equipamentos[i].ultima_manutencao);
            printf("Próxima manutenção: %s\n", lista_equipamentos[i].proxima_manutencao);
            printf("Categoria: %s\n", lista_equipamentos[i].categoria);
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                  VISUALIZAR EQUIPAMENTOS                            ===\n");
        printf("=========================================================================\n");
        printf("===                EQUIPAMENTO NÃO ENCONTRADO                          ===\n");
        printf("=========================================================================\n");
    }

    getchar();
    limparTela();
}
