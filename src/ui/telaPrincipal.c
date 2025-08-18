#include <stdio.h>
#include "header.h"

char telaPrincipal(void) {

    char op;

    printf("\n");
    printf("=================\n");
    printf("    SIG-GYM\n");
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




