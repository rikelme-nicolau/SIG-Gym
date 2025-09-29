#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"

void telaAtualizarFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                    ATUALIZAR FUNCIONÁRIO                          ===\n");
        printf("=========================================================================\n");
        printf("===                   ID  NAO  ENCONTRADO                             ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                    ATUALIZAR FUNCIONÁRIO                          ===\n");
    printf("=========================================================================\n");

    // Listar IDs e nomes apenas dos ativos
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            printf("[%s] %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome);
        }
    }

    printf("===                      DIGITE O ID:                                 ===\n");
    printf("=========================================================================\n");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    // Procurar funcionário pelo ID apenas se estiver ativo
    int encontrado = -1;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        printf("=========================================================================\n");
        printf("===                    ATUALIZAR FUNCIONÁRIO                          ===\n");
        printf("=========================================================================\n");
        printf("===                   ID  NAO  ENCONTRADO                             ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    struct funcionario *func_sel = &lista_funcionarios[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                    ATUALIZAR FUNCIONÁRIO                          ===\n");
        printf("=========================================================================\n");
        printf("Funcionário selecionado: %s (%s)\n", func_sel->nome, func_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n");
        printf("[2] Cargo\n");
        printf("[3] Idade\n");
        printf("[4] CPF\n");
        printf("[5] Endereço\n");
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
            strcpy(func_sel->nome, buffer);
            break;
        case '2':
            limparTela();
            printf("=== Novo cargo: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(func_sel->cargo, buffer);
            break;
        case '3':
            limparTela();
            printf("=== Nova idade: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(func_sel->idade, buffer);
            break;
        case '4':
            limparTela();
            printf("=== Novo CPF: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(func_sel->cpf, buffer);
            break;
        case '5':
            limparTela();
            printf("=== Novo endereço: ===\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(func_sel->endereco, buffer);
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
            printf("===                    ATUALIZAR FUNCIONÁRIO                          ===\n");
            printf("=========================================================================\n");
            printf("=== Atualizado com sucesso ! <enter>                                  ===\n");
            printf("=========================================================================\n");
            getchar();
        }

    } while (opcao != '0');

    limparTela();
}
