#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "src/ui/plano/cadastrarPlano.h"

#include "arquivoAluno.h"  // <-- persistência


#define MAX_BUFFER 1024
#define MAX_ALUNOS 1024


struct aluno
{
    char id[12];
    char nome[MAX_BUFFER];
    char idade[12];
    char cpf[22];
    char telefone[22];
    char endereco[MAX_BUFFER];
    char email[MAX_BUFFER];
    bool ativo;
    char plano_id[12];
};


struct aluno lista_alunos[MAX_ALUNOS];
int total_alunos = 0;

void telaCadastrarAluno(void)
{
    if (total_alunos >= MAX_ALUNOS)
    {
        printf("Limite de alunos atingido!\n");
        getchar();
        return;
    }

    struct aluno novo_aluno;
    char buffer[MAX_BUFFER];

    // Nome
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.nome, buffer);

    // Idade
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite a data de nascimento:                           ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.idade, buffer);

    // CPF
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o CPF:                                          ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.cpf, buffer);

    // Telefone
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o telefone:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.telefone, buffer);

    // Endereço
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o endereco:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.endereco, buffer);

    // Email
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o email:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.email, buffer);


    // Plano


    if (total_planos == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("===                     NENHUM PLANO DISPONIVEL                       ===\n");
        printf("=========================================================================\n");
        strcpy(novo_aluno.plano_id, "0");
        printf(">>>press <ENTER>");
        getchar();
    }
    else
    {
        int algum_ativo = 0;
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("===                     PLANOS DISPONIVEIS                            ===\n");
        printf("=========================================================================\n");
        for (int i = 0; i < total_planos; i++)
        {
            if (lista_planos[i].ativo)
            {
                printf("[%s] %s\n", lista_planos[i].id, lista_planos[i].nome);
                algum_ativo = 1;
            }
        }

        if (algum_ativo)
        {
            printf("=========================================================================\n");
            printf("=== Digite o ID do plano que deseja associar ao aluno:                ===\n");
            printf("=========================================================================\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            int encontrado = 0;
            for (int i = 0; i < total_planos; i++)
            {
                if (strcmp(lista_planos[i].id, buffer) == 0 && lista_planos[i].ativo)
                {
                    strcpy(novo_aluno.plano_id, buffer);
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado)
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        CADASTRAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("===                     PLANO NAO ENCONTRADO                          ===\n");
                printf("=========================================================================\n");
                strcpy(novo_aluno.plano_id, "0");
                printf(">>>press <ENTER>");
                getchar();
            }
        }
        else
        {
            strcpy(novo_aluno.plano_id, "0");
        }
    }

    // Gera ID e define ativo
    snprintf(novo_aluno.id, sizeof(novo_aluno.id), "%d", total_alunos + 1);
    novo_aluno.ativo = true;

    // Adiciona ao vetor e atualiza contador
    lista_alunos[total_alunos++] = novo_aluno;

    // **Salva automaticamente no arquivo**
    salvarAlunos(lista_alunos, total_alunos);

    // Mensagem de sucesso
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                     ALUNO CADASTRADO COM SUCESSO                  ===\n");
    printf("=========================================================================\n");
    printf(">>>press <ENTER>");
    getchar();
    limparTela();
}
