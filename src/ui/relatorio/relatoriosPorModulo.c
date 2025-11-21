#include <stdio.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosPorModulo.h"
#include "relatoriosCruzados.h"
#include "relatoriosAluno.h"
#include "relatoriosPlano.h"
#include "relatoriosEquipamento.h"
#include "relatoriosFuncionario.h"
#include "ui/utils/lerTecla.h"

void moduloRelatoriosPorModulo(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===             RELATORIOS POR MODULO - MENU CENTRAL          ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  RELATORIOS DE ALUNOS                                ===\n");
        printf("===  [2]  RELATORIOS DE PLANOS                                ===\n");
        printf("===  [3]  RELATORIOS DE EQUIPAMENTOS                          ===\n");
        printf("===  [4]  RELATORIOS DE FUNCIONARIOS                          ===\n");
        printf("===  [5]  RELATORIOS CRUZADOS                                 ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

        op = lerTecla();
        limparTela();

        switch (op)
        {
        case '1':
            moduloRelatoriosAluno();
            break;

        case '2':
            moduloRelatoriosPlano();
            break;

        case '3':
            moduloRelatoriosEquipamento();
            break;

        case '4':
            moduloRelatoriosFuncionario();
            break;

        case '5':
            moduloRelatoriosCruzados();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}
