#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"

#define MAX_BUFFER 1024
#define MAX_ALUNOS 1024

struct aluno {
    char id[12];
    char nome[MAX_BUFFER];
    char idade[12];
    char cpf[22];
    char telefone[22];
    char endereco[MAX_BUFFER];
    char email[MAX_BUFFER];
};

struct aluno lista_alunos[MAX_ALUNOS];
int total_alunos = 0; 

void telaCadastrarAluno(void) {
    if(total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    struct aluno novo_aluno;
    char buffer[MAX_BUFFER];

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.nome, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite a data de nascimento:                           ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.idade, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o CPF:                                          ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.cpf, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o telefone:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.telefone, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");   
    printf("=========================================================================\n");
    printf("=== Por favor, digite o endereco:                                     ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.endereco, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o email:                                        ===\n");
    printf("=========================================================================\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.email, buffer);

    snprintf(novo_aluno.id, sizeof(novo_aluno.id), "%d", total_alunos + 1);

    lista_alunos[total_alunos++] = novo_aluno;
    limparTela();
}
