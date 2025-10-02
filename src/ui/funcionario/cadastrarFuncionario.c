#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"
#include "../utils/gerarMatricula.h" //lembrar de voltar 1 pasta para referenciar certo

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;

void telaCadastrarFuncionario(void)
{
    if (total_funcionarios >= MAX_FUNCIONARIOS)
    {
        printf("Limite de funcionários atingido!\n");
        getchar();
        return;
    }

    struct funcionario novo_func;
    char buffer[MAX_BUFFER];

    // Nome
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_func.nome, buffer);

    // Idade
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite a idade:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_func.idade, buffer);

    // CPF
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o CPF:                                          ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_func.cpf, buffer);

    // Endereço
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o endereço:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_func.endereco, buffer);

    // Cargo
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o cargo:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    // Copia com segurança para evitar overflow
    strncpy(novo_func.cargo, buffer, MAX_BUFFER - 1);
    novo_func.cargo[MAX_BUFFER - 1] = '\0';

    // Gera ID(Agora matricula) e define ativo
    // Gera matrícula
    strcpy(novo_func.id, gerarMatricula("001"));
    novo_func.ativo = true;

    // Adiciona ao vetor e atualiza contador
    lista_funcionarios[total_funcionarios++] = novo_func;

    // **Salva automaticamente no arquivo**
    salvarFuncionarios(lista_funcionarios, total_funcionarios);

    // Mensagem de sucesso
    limparTela();
    printf("=========================================================================\n");
    printf("===                   CADASTRAR FUNCIONÁRIO                            ===\n");
    printf("=========================================================================\n");
    printf("===               FUNCIONÁRIO CADASTRADO COM SUCESSO                  ===\n");
    printf("=========================================================================\n");
    printf(">>>press <ENTER>");
    getchar();
    limparTela();
}
