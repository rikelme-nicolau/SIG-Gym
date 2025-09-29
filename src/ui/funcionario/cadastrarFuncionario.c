#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"

// Aqui sim, definimos as variáveis globais
struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;

void telaCadastrarFuncionario(void)
{
    if (total_funcionarios >= MAX_FUNCIONARIOS)
    {
        printf("Limite de funcionarios atingido!\n");
        getchar();
        return;
    }

    struct funcionario novo_funcionario;
    char buffer[MAX_BUFFER];

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_funcionario.nome, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite a idade:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_funcionario.idade, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o CPF:                                          ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_funcionario.cpf, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o endereco:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_funcionario.endereco, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o cargo:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_funcionario.cargo, buffer);

    snprintf(novo_funcionario.id, sizeof(novo_funcionario.id), "%d", total_funcionarios + 1);
    novo_funcionario.ativo = true;

    lista_funcionarios[total_funcionarios++] = novo_funcionario;

    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONARIO                           ===\n");
    printf("=========================================================================\n");
    printf("===             FUNCIONARIO CADASTRADO COM SUCESSO                    ===\n");
    printf("=========================================================================\n");
    printf(">>>press <ENTER>");
    getchar();
    limparTela();
}
