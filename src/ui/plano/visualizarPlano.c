#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"
#include "ui/utils/consoleLayout.h"

#define P_COL_ID 8
#define P_COL_NOME 24
#define P_COL_HORARIO 10
#define P_COL_VALOR 10
#define P_COL_STATUS 8

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

static void tabela_row(const struct plano *pl)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char horario[32];
    snprintf(horario, sizeof(horario), "%s-%s", pl->horario_inicio, pl->horario_fim);
    char valor[32];
    snprintf(valor, sizeof(valor), "R$ %.2f", pl->valor);
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

void telaVisualizarPlano(void)
{
    if (total_planos == 0)
    {
        cabecalho_visualizar("Visualizar planos");
        ui_center_text("Nenhum plano cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_visualizar("Visualizar planos");
    ui_text_line("Planos ativos:");
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
    ui_text_line("Digite o ID do plano que deseja visualizar.");
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
            cabecalho_visualizar("Detalhes do plano");
            info_line("ID", lista_planos[i].id);
            info_line("Nome", lista_planos[i].nome);
            char horario[64];
            snprintf(horario, sizeof(horario), "%s as %s", lista_planos[i].horario_inicio, lista_planos[i].horario_fim);
            info_line("Horario", horario);
            char valor[64];
            snprintf(valor, sizeof(valor), "R$ %.2f", lista_planos[i].valor);
            info_line("Valor", valor);

            ui_section_title("Atividades incluidas");
            if (lista_planos[i].total_atividades == 0)
            {
                ui_text_line("Nenhuma atividade cadastrada.");
            }
            else
            {
                for (int j = 0; j < lista_planos[i].total_atividades; j++)
                {
                    char atividade[UI_INNER + 1];
                    snprintf(atividade, sizeof(atividade), "- %s", lista_planos[i].atividades[j]);
                    ui_text_line(atividade);
                }
            }

            ui_section_title("Pressione <ENTER> para voltar");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        cabecalho_visualizar("Visualizar planos");
        ui_center_text("Plano nao encontrado.");
        ui_section_title("Pressione <ENTER> para voltar");
    }

    getchar();
    limparTela();
}
