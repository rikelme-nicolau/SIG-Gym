#include <stdio.h>



char moduloCheckin(void)
{

    char op;

    printf("\n");
    printf("=================\n");
    printf("=====Check-in=====\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar check-ins \n");
    printf("\n");

    scanf(" %c", &op);

    return op;
}
