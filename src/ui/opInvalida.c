#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/consoleLayout.h"

void opInvalida(void)
{
    ui_section_title("Opcao invalida");
    ui_text_line("Use as teclas indicadas no menu.");
    ui_line('=');
    ui_text_line("Pressione <ENTER> para continuar.");
    getchar();
    limparTela();
}
