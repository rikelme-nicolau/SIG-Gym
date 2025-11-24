#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Menu principal do modulo de planos; retorna a opcao pressionada. */
char telaPlano(void)
{
    char op;

    limparTela();
    ui_header("SIG-GYM", "Modulo de Planos");
    ui_empty_line();
    ui_menu_option('1', "Cadastrar");
    ui_menu_option('2', "Visualizar");
    ui_menu_option('3', "Atualizar");
    ui_menu_option('4', "Excluir");
    ui_empty_line();
    ui_menu_option('0', "Voltar");
    ui_section_title("Escolha uma opcao");
    ui_text_line("Use as teclas indicadas para navegar.");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    /* Usa leitura de tecla unica para evitar lixo no buffer. */
    op = lerTecla();

    limparTela();
    return op;
}
