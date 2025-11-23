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
#include "ui/utils/consoleLayout.h"

void moduloRelatoriosPorModulo(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios por Modulo");
        ui_section_title("Relatorios disponiveis");
        ui_menu_option('1', "Relatorios de alunos");
        ui_menu_option('2', "Relatorios de planos");
        ui_menu_option('3', "Relatorios de equipamentos");
        ui_menu_option('4', "Relatorios de funcionarios");
        ui_menu_option('5', "Relatorios cruzados");
        ui_empty_line();
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        ui_text_line("Use as teclas indicadas para navegar.");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

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
