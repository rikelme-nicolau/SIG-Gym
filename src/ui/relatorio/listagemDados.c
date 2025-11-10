#include <stdio.h>

#include "limparTela.h"
#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"

void relatorioListagemDados(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                 RELATÓRIO - LISTAGEM DE DADOS                     ===\n");
    printf("=========================================================================\n\n");

    int alunos_ativos = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunos_ativos++;
        }
    }
    printf("ALUNOS ATIVOS: %d\n", alunos_ativos);
    if (alunos_ativos > 0)
    {
        for (int i = 0; i < total_alunos; i++)
        {
            if (lista_alunos[i].ativo)
            {
                printf("  - [%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
            }
        }
    }
    else
    {
        printf("  Nenhum aluno ativo encontrado.\n");
    }

    printf("\n");

    int planos_ativos = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            planos_ativos++;
        }
    }
    printf("PLANOS ATIVOS: %d\n", planos_ativos);
    if (planos_ativos > 0)
    {
        for (int i = 0; i < total_planos; i++)
        {
            if (lista_planos[i].ativo)
            {
                printf("  - [%s] %s\n", lista_planos[i].id, lista_planos[i].nome);
            }
        }
    }
    else
    {
        printf("  Nenhum plano ativo encontrado.\n");
    }

    printf("\n");

    int funcionarios_ativos = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            funcionarios_ativos++;
        }
    }
    printf("FUNCIONÁRIOS ATIVOS: %d\n", funcionarios_ativos);
    if (funcionarios_ativos > 0)
    {
        for (int i = 0; i < total_funcionarios; i++)
        {
            if (lista_funcionarios[i].ativo)
            {
                printf("  - [%s] %s (%s)\n", lista_funcionarios[i].id, lista_funcionarios[i].nome, lista_funcionarios[i].cargo);
            }
        }
    }
    else
    {
        printf("  Nenhum funcionário ativo encontrado.\n");
    }

    printf("\n");

    int equipamentos_ativos = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            equipamentos_ativos++;
        }
    }
    printf("EQUIPAMENTOS ATIVOS: %d\n", equipamentos_ativos);
    if (equipamentos_ativos > 0)
    {
        for (int i = 0; i < total_equipamentos; i++)
        {
            if (lista_equipamentos[i].ativo)
            {
                printf("  - [%s] %s - Próx. manutenção: %s\n", lista_equipamentos[i].id, lista_equipamentos[i].nome, lista_equipamentos[i].proxima_manutencao);
            }
        }
    }
    else
    {
        printf("  Nenhum equipamento ativo encontrado.\n");
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER> para voltar...");
    getchar();

    limparTela();
}
