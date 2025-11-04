#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui/funcionario/cadastrarFuncionario.h"
#include "ui/aluno/cadastrarAluno.h"
#include "limparTela.h"
#include "ui/utils/validarNascimento.h"

void buscarPorCPF(void)
{
    limparTela();
    printf("=========================================================================\n");
    printf("===                         BUSCAR POR CPF                             ===\n");
    printf("=========================================================================\n");

    char cpf_busca[22];
    printf(">>> Digite o CPF que deseja buscar: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

    int encontrado = 0;

    printf("\n");

    // ==========================
    // Buscar em FUNCIONÁRIOS
    // ==========================
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo && strcmp(lista_funcionarios[i].cpf, cpf_busca) == 0)
        {
            printf("=========================================================================\n");
            printf("===                  PERFIL ENCONTRADO: FUNCIONÁRIO                    ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_funcionarios[i].id);
            printf("Nome: %s\n", lista_funcionarios[i].nome);
            printf("Cargo: %s\n", lista_funcionarios[i].cargo);
            int idade = calcularIdade(lista_funcionarios[i].nascimento);
            printf("Idade: %d\n", idade);
            printf("CPF: %s\n", lista_funcionarios[i].cpf);
            printf("Telefone: %s\n", lista_funcionarios[i].telefone);
            printf("Endereço: %s\n", lista_funcionarios[i].endereco);
            printf("Email: %s\n", lista_funcionarios[i].email);
            printf("=========================================================================\n\n");
            encontrado = 1;
        }
    }

    // ==========================
    // Buscar em ALUNOS
    // ==========================
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo && strcmp(lista_alunos[i].cpf, cpf_busca) == 0)
        {
            printf("=========================================================================\n");
            printf("===                     PERFIL ENCONTRADO: ALUNO                       ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_alunos[i].id);
            printf("Nome: %s\n", lista_alunos[i].nome);
            printf("Idade: %s\n", lista_alunos[i].idade);
            printf("CPF: %s\n", lista_alunos[i].cpf);
            printf("Telefone: %s\n", lista_alunos[i].telefone);
            printf("Endereço: %s\n", lista_alunos[i].endereco);
            printf("Email: %s\n", lista_alunos[i].email);
            printf("Plano ID: %s\n", lista_alunos[i].plano_id);
            printf("=========================================================================\n\n");
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                        NENHUM CPF ENCONTRADO                       ===\n");
        printf("=========================================================================\n");
    }

    printf(">>> Pressione <ENTER> para continuar...");
    getchar();
    limparTela();
}
