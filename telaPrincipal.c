#include <stdio.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
>>>>>>> 14d5134d0813851226a6b595b271c414a5a596de

char telaPrincipal(void) {

    char op;

    printf("\n");
    printf("=================\n");
    printf("=====SIG-GYM=====\n");
    printf("=================\n");
    printf("(1) Alunos \n");
    printf("(2) Planos \n");
    printf("(3) Matriculas \n");
    printf("(4) Check-ins \n");
    printf("(5) Sobre \n");
    printf("\n");

    scanf(" %c", &op);

    return op;
    }

    
    char moduloAluno() {

    printf("=================================\n");
    printf("|         MÓDULO ALUNO          |\n");
    printf("=================================\n");
    printf("| 1. Cadastrar Novo Aluno       |\n");
    printf("| 2. Atualizar Dados do Aluno   |\n");
    printf("| 3. Visualizar Dados do Aluno  |\n");
    printf("| 4. Deletar Aluno              |\n");
    printf("| 0. Sair                       |\n");
    printf("=================================\n");

    char op_aluno;
    printf("O que deseja fazer? Escolha uma das opções acima: ");
    scanf(" %c", &op_aluno); 

    return op_aluno;
}



