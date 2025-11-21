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
    printf("===  [1]  ALUNO x PLANO x VALOR                                      ===\n");
    printf("===  [2]  PLANO x HORARIO x ALUNOS                                   ===\n");
    printf("===  [3]  EQUIPAMENTO x CATEGORIA x MANUTENCAO                        ===\n");
    printf("===  [4]  FUNCIONARIO x CARGO x FAIXA ETARIA                          ===\n");
    printf("===  [5]  ALUNO x IDADE x PLANO                                      ===\n");
    printf("===  [6]  PLANO x ATIVIDADE x POPULARIDADE                           ===\n");
    printf("===                                                                   ===\n");
    printf("===  [0]  VOLTAR                                                      ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    op = lerTecla();

    limparTela();

    return op;
}
