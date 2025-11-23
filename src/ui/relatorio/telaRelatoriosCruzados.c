#include <stdio.h>

#include "limparTela.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

#include "telaRelatoriosCruzados.h"

char telaRelatoriosCruzados(void)
{
    char op;

    limparTela();
    ui_header("SIG-GYM", "Relatorios Cruzados");
    ui_empty_line();
    ui_menu_option('1', "Aluno x Plano x Valor");
    ui_menu_option('2', "Plano x Horario x Alunos");
    ui_menu_option('3', "Equipamento x Categoria x Manutencao");
    ui_menu_option('4', "Funcionario x Cargo x Faixa Etaria");
    ui_menu_option('5', "Aluno x Idade x Plano");
    ui_menu_option('6', "Plano x Atividade x Popularidade");
    ui_menu_option('7', "Todas as pessoas (alunos + funcionarios)");
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
