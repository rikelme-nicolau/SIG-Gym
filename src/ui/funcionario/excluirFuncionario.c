#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"
#include "ui/utils/consoleLayout.h"

#define FC_COL_ID 10
#define FC_COL_NOME 22
#define FC_COL_CARGO 14
#define FC_COL_STATUS 8

static void cabecalho_excluir(const char *sub)
{
    limparTela();
    ui_header("SIG-GYM", sub);
    ui_empty_line();
}

static bool ler_linha(char *dest, size_t size)
{
    if (fgets(dest, size, stdin) == NULL)
    {
        dest[0] = '\0';
        return false;
    }
    dest[strcspn(dest, "\n")] = '\0';
    return true;
}

static void tabela_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", FC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", FC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Cargo", FC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", FC_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_row(const struct funcionario *f)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, f->id, FC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->nome, FC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->cargo, FC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->ativo ? "Ativo" : "Inativo", FC_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void telaExcluirFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        cabecalho_excluir("Excluir funcionario");
        ui_center_text("Nenhum funcionario cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_excluir("Excluir funcionario");
    ui_text_line("Selecione o funcionario ativo para excluir.");
    tabela_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            tabela_row(&lista_funcionarios[i]);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        ui_section_title("Nenhum funcionario ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do funcionario que deseja excluir.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[32];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            lista_funcionarios[i].ativo = false;
            excluirFuncionario(id_busca);

            cabecalho_excluir("Excluir funcionario");
            ui_center_text("Funcionario excluido com sucesso.");
            ui_section_title("Pressione <ENTER> para voltar");
            getchar();
            limparTela();
            return;
        }
    }

    cabecalho_excluir("Excluir funcionario");
    ui_center_text("Funcionario nao encontrado.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
