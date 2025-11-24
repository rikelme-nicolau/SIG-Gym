#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"
#include "ui/utils/consoleLayout.h"

/* Tela de exclusao logica: lista equipamentos ativos, solicita um ID e marca como inativo. */

#define EQ_COL_ID 8
#define EQ_COL_NOME 22
#define EQ_COL_CATEG 14
#define EQ_COL_DATA 10
#define EQ_COL_STATUS 6

static void cabecalho_excluir(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Leitura de linha simples para evitar lixo no buffer. */
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

/* Cabecalho da tabela de selecao. */
static void tabela_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", EQ_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", EQ_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Categoria", EQ_COL_CATEG);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Ult. Manut", EQ_COL_DATA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", EQ_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

/* Linha resumida do equipamento na listagem de exclusao. */
static void tabela_row(const struct equipamento *eq)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, eq->id, EQ_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->nome, EQ_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->categoria, EQ_COL_CATEG);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->ultima_manutencao, EQ_COL_DATA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->ativo ? "Ativo" : "Inativo", EQ_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

/* Fluxo principal: confirma ID informado e grava exclusao logica no arquivo. */
void telaExcluirEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        cabecalho_excluir("Excluir equipamento");
        ui_center_text("Nenhum equipamento cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_excluir("Excluir equipamento");
    ui_text_line("Selecione o equipamento ativo para excluir.");
    tabela_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            tabela_row(&lista_equipamentos[i]);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        ui_section_title("Nenhum equipamento ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do equipamento que deseja excluir.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[12];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            lista_equipamentos[i].ativo = false;
            excluirEquipamento(id_busca);

            cabecalho_excluir("Excluir equipamento");
            ui_center_text("Equipamento excluido com sucesso.");
            ui_section_title("Pressione <ENTER> para voltar");
            getchar();
            limparTela();
            return;
        }
    }

    cabecalho_excluir("Excluir equipamento");
    ui_center_text("Equipamento nao encontrado.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
