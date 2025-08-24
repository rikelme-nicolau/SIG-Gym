#include <stdio.h>

char telaPlano(void) {

char op;

    printf("\n");
    
    printf("=================\n");
    printf("    Planos\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar\n");
    printf("(3) Atualizar\n");
    printf("(4) Excluir\n");
    printf("(0) Sair\n");
    printf("\n");

    scanf(" %c", &op);

    return op;
}