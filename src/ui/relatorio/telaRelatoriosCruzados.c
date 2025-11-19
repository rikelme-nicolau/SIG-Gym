#include <stdio.h>

#include "limparTela.h"
#include "ui/utils/lerTecla.h"

#include "telaRelatoriosCruzados.h"

char telaRelatoriosCruzados(void)
{
    char op;

    printf("\n");
    printf("=========================================================================\n");
    printf("===                      RELATÓRIOS CRUZADOS                          ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  RELAÇÃO ALUNO / PLANO                                      ===\n");
    printf("===                                                                   ===\n");
    printf("===  [0]  VOLTAR                                                      ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();

    return op;
}
