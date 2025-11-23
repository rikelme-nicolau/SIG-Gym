#include <stdio.h>
#include "limparTela.h"
#include "ui/utils/consoleLayout.h"

void telaFinalizacao(void)
{
    limparTela();
    ui_header("SIG-GYM", "Fim do programa");
    ui_empty_line();
    ui_text_line("Obrigado por utilizar o SIG-GYM!");
    ui_text_line("Bacharelado em Sistemas da Informacao");
    ui_text_line("DCT1106 -- Programacao");
    ui_text_line("Centro de Ensino Superior do Serido");
    ui_text_line("Departamento de Computacao e Tecnologia");
    ui_text_line("Rikelme Nicolau Borges do Nascimento");
    ui_line('-');
    ui_section_title("Pressione <ENTER> para finalizar");
    getchar();
    limparTela();
}
