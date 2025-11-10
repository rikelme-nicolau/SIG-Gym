#include <stdio.h>

#include "limparTela.h"

char telaRelatorio(void)
{
    char op;

    printf("\n");
    printf("=========================================================================\n");
    printf("===                          RELATÓRIOS                               ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  RELATÓRIO DE LISTAGEM DE DADOS                              ===\n");
    printf("===                                                                   ===\n");
    printf("===  [0]  VOLTAR                                                      ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    scanf("%c", &op);
    getchar();

    limparTela();

    return op;
}
