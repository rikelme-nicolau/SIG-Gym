#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

char telaPrincipal(void)
{

    char op;

    limparTela();
    ui_header("SIG-GYM", "Menu principal");
    ui_empty_line();
    ui_menu_option('1', "Alunos");
    ui_menu_option('2', "Planos");
    ui_menu_option('3', "Equipamentos");
    ui_menu_option('4', "Funcionarios");
    ui_menu_option('5', "Relatorios");
    ui_menu_option('6', "Sobre");
    ui_empty_line();
    ui_menu_option('0', "Sair");
    ui_section_title("Escolha uma opcao");
    ui_text_line("Use as teclas indicadas para navegar.");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    op = lerTecla();

    limparTela();
    return op;
}
