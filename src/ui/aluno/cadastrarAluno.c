#include <stdio.h>
#include <string.h>
#include "C:\Users\conta\Documents\vs_sig_gym\SIG-Gym\src\header_tela.h"

void inserirDados(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    int tam = strlen(str);

    if (str[tam-1] == '\n'){
        str[tam-1] = '\0';
    }

}

void telaCadastrarAluno(void) {
    char nome[60];
    char cpf[20];
    char dataNascimento[15];
    char telefone[20];
    char email[60];
    
    printf("\n");
    printf("=================\n");
    printf(" CADASTRAR ALUNO\n");
    printf("=================\n");
    printf("(1) Nome completo\n");
    printf("(2) CPF\n");
    printf("(3) Data de nascimento\n");
    printf("(4) Telefone\n");
    printf("(5) Email\n");
    printf("\n");

    printf("Insira o nome completo do aluno:\n");
    inserirDados(nome, 60);

    printf("Insira o CPF do aluno:\n");
    inserirDados(cpf, 20);
    
    printf("Insira a data de nascimento do aluno:\n");
    inserirDados(dataNascimento, 15);

    printf("Insira o telefone do aluno:\n");
    inserirDados(telefone, 20);

    printf("Insira o email do aluno:\n");
    inserirDados(email, 20);

    printf("==========================\n"); //print temporário, apenas para mostrar funcionalidade
    printf("    Dados Cadastrados\n");
    printf("==========================\n");
    printf("(1) %s\n",nome);
    printf("(2) %s\n", cpf);
    printf("(3) %s\n", dataNascimento);
    printf("(4) %s\n", telefone);
    printf("(5) %s\n", email);

    printf("Dados cadastrados com sucesso, pressione |ENTER| para continuar..");
    getchar();

}
