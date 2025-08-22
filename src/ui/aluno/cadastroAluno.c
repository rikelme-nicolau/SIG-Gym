#include <stdio.h>
#include <string.h>

void cadastrarAluno(void) {
    char nome[60];
    char cpf[20];
    char dataNascimento[15];
    char telefone[20];
    char email[60];

    printf("===========================\n");
    printf("    Dados para Cadastro\n");
    printf("===========================\n");
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

    return 0;

}