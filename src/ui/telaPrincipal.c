#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/lerTecla.h"

char telaPrincipal(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                              SIG-GYM                              ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  ALUNOS                                                      ===\n");
    printf("===  [2]  PLANOS                                                      ===\n");
    printf("===  [3]  EQUIPAMENTOS                                                ===\n");
    printf("===  [4]  FUNCIONARIOS                                                ===\n");
    printf("===  [5]  RELATORIOS                                                  ===\n");
    printf("===  [6]  SOBRE                                                       ===\n");
    printf("===  [0]  SAIR                                                        ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();
    return op;
}
