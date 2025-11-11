#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/lerTecla.h"

char telaEquipamento(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                          EQUIPAMENTO                              ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  CADASTRAR                                                   ===\n");
    printf("===  [2]  VISUALIZAR                                                  ===\n");
    printf("===  [3]  ATUALIZAR                                                   ===\n");
    printf("===  [4]  EXCLUIR                                                     ===\n");
    printf("===  [0]  SAIR                                                        ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();
    return op;
}
