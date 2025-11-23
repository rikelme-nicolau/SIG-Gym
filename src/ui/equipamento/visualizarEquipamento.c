#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "ui/utils/consoleLayout.h"

#define EQ_COL_ID 8
#define EQ_COL_NOME 22
#define EQ_COL_CATEG 14
#define EQ_COL_DATA 10
#define EQ_COL_STATUS 6

static void cabecalho_visualizar(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
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

static void info_line(const char *label, const char *value)
{
    char linha[UI_INNER + 1];
    size_t prefix = strlen(label) + 2;
    int max_val = (int)UI_INNER - (int)prefix;
    if (max_val < 0)
    {
        max_val = 0;
    }
    char safe[UI_INNER + 1];
    ui_clip_utf8(value, max_val, safe, sizeof(safe));
    snprintf(linha, sizeof(linha), "%s: %-.*s", label, max_val, safe);
    ui_text_line(linha);
}

void telaVisualizarEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        cabecalho_visualizar("Visualizar equipamentos");
        ui_center_text("Nenhum equipamento cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_visualizar("Visualizar equipamentos");
    ui_text_line("Equipamentos ativos:");
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
    ui_text_line("Digite o ID do equipamento que deseja visualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[12];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            cabecalho_visualizar("Detalhes do equipamento");
            info_line("ID", lista_equipamentos[i].id);
            info_line("Nome", lista_equipamentos[i].nome);
            info_line("Ultima manutencao", lista_equipamentos[i].ultima_manutencao);
            info_line("Proxima manutencao", lista_equipamentos[i].proxima_manutencao);
            info_line("Categoria", lista_equipamentos[i].categoria);
            ui_section_title("Pressione <ENTER> para voltar");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        cabecalho_visualizar("Visualizar equipamentos");
        ui_center_text("Equipamento nao encontrado.");
        ui_section_title("Pressione <ENTER> para voltar");
    }

    getchar();
    limparTela();
}
