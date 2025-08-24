#include <stdio.h>

char telaMatricula(void)
{

    char op;

    printf("\n");
    printf("=================\n");
    printf("    Matriculas\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar\n");
    printf("(3) Atualizar\n");
    printf("(4) Excluir\n");

    scanf(" %c", &op);

    return op;
}
