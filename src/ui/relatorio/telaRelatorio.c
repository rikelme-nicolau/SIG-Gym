#include <stdio.h>

#include "limparTela.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Tela antiga de menu de relatorios (mantida para compatibilidade). */
char telaRelatorio(void)
{
    char op;

    limparTela();
    ui_header("SIG-GYM", "Relatorios - Menu");
    ui_empty_line();
    ui_menu_option('1', "Relatorios gerenciais");
    ui_menu_option('2', "Relatorios operacionais");
    ui_menu_option('3', "Relatorios por modulo");
    ui_menu_option('4', "Relatorios personalizados");
    ui_menu_option('5', "Exportar relatorios");
    ui_empty_line();
    ui_menu_option('0', "Voltar");
    ui_section_title("Escolha uma opcao");
    ui_text_line("Use as teclas indicadas para navegar.");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    op = lerTecla();

    limparTela();

    return op;
}
