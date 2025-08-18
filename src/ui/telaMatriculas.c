#include <stdio.h>
#include "header.h"


char telaMatriculas(void)
{

    char op;

    printf("\n");
    printf("=================\n");
    printf("    Matriculas\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar matriculas \n");
    printf("\n");

    scanf(" %c", &op);

    return op;
}
