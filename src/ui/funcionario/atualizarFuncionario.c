#include <stdio.h>
#include "limparTela.h"
#include "atualizarFuncionario.h"
char telaAtualizarFuncionario(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                        ATUALIZAR FUNCIONÁRIO                      ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
}
