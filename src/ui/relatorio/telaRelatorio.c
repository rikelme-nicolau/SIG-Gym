#include <stdio.h>

#include "limparTela.h"
#include "ui/utils/lerTecla.h"

char telaRelatorio(void)
{
    char op;

    printf("\n");
    printf("=========================================================================\n");
    printf("===                          RELATÓRIOS                               ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  RELATÓRIO DE LISTAGEM DE DADOS                              ===\n");
    printf("===  [2]  RELATÓRIO COM APLICAÇÃO DE FILTRO                           ===\n");
    printf("===                                                                   ===\n");
    printf("===  [0]  VOLTAR                                                      ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();

    return op;
}
