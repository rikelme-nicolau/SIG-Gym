#include <stdio.h>
#include "header.h"

char telaPlano(void) {

char op;

    printf("\n");
    
    printf("=================\n");
    printf("    Planos\n");
    printf("=================\n");
    printf("(1) Cadastrar novo plano\n");
    printf("(2) Visualizar planos\n");
    
    printf("\n");

    scanf(" %c", &op);

    return op;
}