#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"
#include "arquivoPlano.h"
#include "ui/utils/consoleLayout.h"

/* Tela de exclusao logica: lista planos ativos, pede um ID e marca como inativo. */

#define P_COL_ID 8
#define P_COL_NOME 24
#define P_COL_HORARIO 10
#define P_COL_VALOR 10
#define P_COL_STATUS 8

static void cabecalho_excluir(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Entrada de linha basica para nao deixar lixo no buffer. */
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

/* Cabecalho da tabela de listagem usado nesta tela. */
static void tabela_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", P_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", P_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Horario", P_COL_HORARIO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Valor", P_COL_VALOR);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", P_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

/* Linha com informacoes resumidas do plano. */
static void tabela_row(const struct plano *pl)
{
    char horario[32];
    snprintf(horario, sizeof(horario), "%s-%s", pl->horario_inicio, pl->horario_fim);
    char valor[32];
    snprintf(valor, sizeof(valor), "R$ %.2f", pl->valor);
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, pl->id, P_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pl->nome, P_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, horario, P_COL_HORARIO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, valor, P_COL_VALOR);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pl->ativo ? "Ativo" : "Inativo", P_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

/* Fluxo principal: confirma ID e marca o plano como inativo no arquivo. */
void telaExcluirPlano(void)
{
    if (total_planos == 0)
    {
        cabecalho_excluir("Excluir plano");
        ui_center_text("Nenhum plano cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_excluir("Excluir plano");
    ui_text_line("Selecione o plano ativo para excluir.");
    tabela_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            tabela_row(&lista_planos[i]);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        ui_section_title("Nenhum plano ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do plano que deseja excluir.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[12];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (strcmp(lista_planos[i].id, id_busca) == 0 && lista_planos[i].ativo)
        {
            lista_planos[i].ativo = false;
            excluirPlano(id_busca);

            cabecalho_excluir("Excluir plano");
            ui_center_text("Plano excluido com sucesso.");
            ui_section_title("Pressione <ENTER> para voltar");
            getchar();
            limparTela();
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        cabecalho_excluir("Excluir plano");
        ui_center_text("Plano nao encontrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
    }
}
