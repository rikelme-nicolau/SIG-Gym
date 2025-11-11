#include <stdio.h>
#include "limparTela.h"
#include "telaFuncionario.h"
#include "ui/utils/lerTecla.h"
char telaFuncionario(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                              FUNCIONARIO                          ===\n");
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
