#include <stdio.h>
#include "limparTela.h"
#include "excluirFuncionario.h"
char telaExcluirFuncionario(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                        EXCLUIR FUNCIONÁRIO                        ===\n");
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
