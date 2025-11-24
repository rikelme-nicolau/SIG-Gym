#include <stdio.h>
#include "limparTela.h"
#include "atualizarAluno.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Monta o menu de navegacao do modulo de alunos e retorna a opcao escolhida.
   Toda formatacao de layout e delegada para os helpers de console. */
char telaAluno(void)
{
    char op;

    limparTela();
    ui_header("SIG-GYM", "Modulo de Alunos");
    ui_empty_line();
    ui_menu_option('1', "Cadastrar");
    ui_menu_option('2', "Visualizar");
    ui_menu_option('3', "Atualizar");
    ui_menu_option('4', "Excluir");
    ui_empty_line();
    ui_menu_option('0', "Voltar");
    ui_section_title("Escolha uma opcao");
    ui_text_line("Use as teclas indicadas acima para navegar.");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    /* Usa leitura de tecla unica para evitar problemas com lixo de buffer. */
    op = lerTecla();

    limparTela();
    return op;
}
