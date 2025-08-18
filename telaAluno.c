#include <stdio.h>

char moduloAluno(void) {

char op;

    printf("\n");
    
    printf("=================\n");
    printf("    Aluno\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar alunos\n");
    
    printf("\n");

    scanf(" %c", &op);

    return op;

}