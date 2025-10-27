#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "src/ui/utils/validarNascimento.h"

void telaVisualizarFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                    VISUALIZAR FUNCIONÁRIO                         ===\n");
        printf("=========================================================================\n");
        printf("===               NENHUM FUNCIONÁRIO CADASTRADO                       ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                    VISUALIZAR FUNCIONÁRIO                         ===\n");
    printf("=========================================================================\n");

    // Listar apenas funcionários ativos
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
        printf("===               NENHUM FUNCIONÁRIO ATIVO                            ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do funcionário que deseja visualizar: ");
    char id_busca[1024];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                    INFORMAÇÕES DO FUNCIONÁRIO                     ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_funcionarios[i].id);
            printf("Nome: %s\n", lista_funcionarios[i].nome);
            printf("Cargo: %s\n", lista_funcionarios[i].cargo);

            // Calcula a idade usando a data de nascimento
            int idade = calcularIdade(lista_funcionarios[i].nascimento);
            printf("Idade: %d\n", idade);

            printf("CPF: %s\n", lista_funcionarios[i].cpf);
            printf("Endereço: %s\n", lista_funcionarios[i].endereco);

            printf("=========================================================================\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                    VISUALIZAR FUNCIONÁRIO                         ===\n");
        printf("=========================================================================\n");
        printf("===               NENHUM FUNCIONÁRIO ENCONTRADO                       ===\n");
        printf("=========================================================================\n");
    }

    printf(">>> Pressione <ENTER> para continuar...");
    getchar();
    limparTela();
}
