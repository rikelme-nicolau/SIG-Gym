#include <stdio.h>

#include "limparTela.h"
#include "ui/utils/lerTecla.h"

char telaRelatorio(void)
{
    char op;

    printf("\n");
    printf("=========================================================================\n");
    printf("===                        RELATORIOS - MENU                          ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  RELATORIOS GERENCIAIS                                       ===\n");
    printf("===  [2]  RELATORIOS OPERACIONAIS                                     ===\n");
    printf("===  [3]  RELATORIOS POR MODULO                                       ===\n");
    printf("===  [4]  RELATORIOS PERSONALIZADOS                                   ===\n");
    printf("===  [5]  EXPORTAR RELATORIOS                                         ===\n");
    printf("===                                                                   ===\n");
    printf("===  [0]  VOLTAR                                                      ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();

    return op;
}
