#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"

#define MAX_BUFFER 1024

void telaAtualizarEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                   ATUALIZAR EQUIPAMENTO                             ===\n");
        printf("=========================================================================\n");
        printf("===                   NENHUM EQUIPAMENTO CADASTRADO                     ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                   ATUALIZAR EQUIPAMENTO                             ===\n");
    printf("=========================================================================\n");

    // Lista equipamentos ativos
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            printf("[%s] %s\n", lista_equipamentos[i].id, lista_equipamentos[i].nome);
        }
    }

    printf("===                          DIGITE O ID:                             ===\n");
    printf("=========================================================================\n");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        printf("=========================================================================\n");
        printf("===                   ATUALIZAR EQUIPAMENTO                             ===\n");
        printf("=========================================================================\n");
        printf("===                   ID NÃO ENCONTRADO                                 ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    struct equipamento *equip_sel = &lista_equipamentos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                   ATUALIZAR EQUIPAMENTO                             ===\n");
        printf("=========================================================================\n");
        printf("Equipamento selecionado: %s (%s)\n", equip_sel->nome, equip_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n");
        printf("[2] Última manutenção\n");
        printf("[3] Categoria\n");
        printf("[0] Voltar\n");
        printf("=========================================================================\n");
        scanf(" %c", &opcao);
        getchar();

        switch (opcao)
        {
        case '1':
            limparTela();
            printf("=== Novo nome: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(equip_sel->nome, buffer);
            atualizarEquipamentoNoArquivo(*equip_sel);
            break;

        case '2':
            limparTela();
            printf("=== Nova data da última manutenção (dd/mm/aaaa): ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(equip_sel->ultima_manutencao, buffer);

            // Atualiza automaticamente a próxima manutenção
            calcularProximaManutencao(equip_sel->ultima_manutencao, equip_sel->proxima_manutencao);

            atualizarEquipamentoNoArquivo(*equip_sel);
            break;

        case '3':
            limparTela();
            printf("=== Nova categoria: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(equip_sel->categoria, buffer);
            atualizarEquipamentoNoArquivo(*equip_sel);
            break;

        case '0':
            break;

        default:
            limparTela();
            printf("=== OPÇÃO INVÁLIDA ===\n");
            break;
        }

        if (opcao != '0')
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                   ATUALIZAR EQUIPAMENTO                             ===\n");
            printf("=========================================================================\n");
            printf("=== Atualizado com sucesso! <ENTER>                                     ===\n");
            printf("=========================================================================\n");
            getchar();
        }

    } while (opcao != '0');

    limparTela();
}
