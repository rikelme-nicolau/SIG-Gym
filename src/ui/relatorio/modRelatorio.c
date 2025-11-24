#include <stdio.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosGerenciais.h"
#include "relatoriosOperacionais.h"
#include "relatoriosPorModulo.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Menu raiz do modulo de relatorios: encaminha o usuario para
   conjuntos de relatorios (gerenciais, operacionais ou por modulo). */
char moduloRelatorio(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios - Menu Principal");
        ui_empty_line();
        ui_menu_option('1', "Relatorios gerenciais");
        ui_menu_option('2', "Relatorios operacionais");
        ui_menu_option('3', "Relatorios por modulo");
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
            moduloRelatoriosGerenciais();
            break;

        case '2':
            moduloRelatoriosOperacionais();
            break;

        case '3':
            moduloRelatoriosPorModulo();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

        /*
        // Opcoes anteriores mantidas temporariamente:
        case '1':
            relatorioListagemDados();
            break;

        case '2':
            relatorioListagemFiltrada();
            break;

        case '3':
            moduloRelatoriosCruzados();
            break;
        */

    } while (op != '0');

    return op;
}
