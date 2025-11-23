#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h" // Para acessar lista_planos e total_planos
#include "ui/utils/consoleLayout.h"

static void cabecalho_visualizar(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

// Larguras que somam 63 caracteres; com 3 separadores " | " = 72 (UI_INNER)
#define COL_ID 12
#define COL_NOME 26
#define COL_PLANO 17
#define COL_STATUS 8

static void tabela_alunos_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_alunos_row(const char *id, const char *nome, const char *plano, const char *status)
{
    char linha[UI_INNER + 1];
    char status_clip[32];
    ui_clip_utf8(status, COL_STATUS, status_clip, sizeof(status_clip));
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, id, COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nome, COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, plano, COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status_clip, COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void info_line(const char *label, const char *value)
{
    char linha[UI_INNER + 1];
    char safe[UI_INNER + 1];
    size_t prefix = strlen(label) + 2; // label + ": "
    int max_val = (int)UI_INNER - (int)prefix;
    if (max_val < 0)
    {
        max_val = 0;
    }
    ui_clip_utf8(value, max_val, safe, sizeof(safe));
    snprintf(linha, sizeof(linha), "%s: %-.*s", label, max_val, safe);
    ui_text_line(linha);
}

void telaVisualizarAluno(void)
{
    if (total_alunos == 0)
    {
        cabecalho_visualizar("Visualizar alunos");
        ui_center_text("Nenhum aluno cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_visualizar("Visualizar alunos");
    ui_text_line("Alunos ativos:");
    tabela_alunos_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            const char *status = "Ativo";
            char nome_plano[MAX_BUFFER] = "Sem plano";
            if (strcmp(lista_alunos[i].plano_id, "0") != 0)
            {
                for (int j = 0; j < total_planos; j++)
                {
                    if (lista_planos[j].ativo && strcmp(lista_planos[j].id, lista_alunos[i].plano_id) == 0)
                    {
                        strncpy(nome_plano, lista_planos[j].nome, sizeof(nome_plano) - 1);
                        nome_plano[sizeof(nome_plano) - 1] = '\0';
                        break;
                    }
                }
            }

            tabela_alunos_row(lista_alunos[i].id, lista_alunos[i].nome, nome_plano, status);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        ui_section_title("Nenhum aluno ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do aluno que deseja visualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[1024];
    if (fgets(id_busca, sizeof(id_busca), stdin) == NULL)
    {
        limparTela();
        return;
    }
    id_busca[strcspn(id_busca, "\r\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (strcmp(lista_alunos[i].id, id_busca) == 0 && lista_alunos[i].ativo)
        {
            cabecalho_visualizar("Informacoes do aluno");
            info_line("ID", lista_alunos[i].id);
            info_line("Nome", lista_alunos[i].nome);
            info_line("Data de nascimento", lista_alunos[i].idade);
            info_line("CPF", lista_alunos[i].cpf);
            info_line("Telefone", lista_alunos[i].telefone);
            info_line("Endereco", lista_alunos[i].endereco);
            info_line("Email", lista_alunos[i].email);

            // Mostrar nome do plano associado
            char nome_plano[MAX_BUFFER] = "Nenhum";
            if (strcmp(lista_alunos[i].plano_id, "0") != 0)
            {
                for (int j = 0; j < total_planos; j++)
                {
                    if (lista_planos[j].ativo && strcmp(lista_planos[j].id, lista_alunos[i].plano_id) == 0)
                    {
                        strcpy(nome_plano, lista_planos[j].nome);
                        break;
                    }
                }
            }
            info_line("Plano associado", nome_plano);

            ui_section_title("Pressione <ENTER> para voltar");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        cabecalho_visualizar("Visualizar alunos");
        ui_center_text("Nenhum aluno encontrado com esse ID.");
        ui_section_title("Pressione <ENTER> para voltar");
    }

    getchar();
    limparTela();
}
