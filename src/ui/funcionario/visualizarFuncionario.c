#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "src/ui/utils/validarNascimento.h"
#include "ui/utils/consoleLayout.h"

#define FC_COL_ID 10
#define FC_COL_NOME 22
#define FC_COL_CARGO 14
#define FC_COL_STATUS 8

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

void telaVisualizarFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        cabecalho_visualizar("Visualizar funcionarios");
        ui_center_text("Nenhum funcionario cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_visualizar("Visualizar funcionarios");
    ui_text_line("Funcionarios ativos:");
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
    ui_text_line("Digite o ID do funcionario que deseja visualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[1024];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            cabecalho_visualizar("Informacoes do funcionario");
            info_line("ID", lista_funcionarios[i].id);
            info_line("Nome", lista_funcionarios[i].nome);
            info_line("Cargo", lista_funcionarios[i].cargo);
            int idade = calcularIdade(lista_funcionarios[i].nascimento);
            char idade_str[32];
            snprintf(idade_str, sizeof(idade_str), "%d", idade);
            info_line("Idade", idade_str);
            info_line("CPF", lista_funcionarios[i].cpf);
            info_line("Endereco", lista_funcionarios[i].endereco);
            ui_section_title("Pressione <ENTER> para voltar");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        cabecalho_visualizar("Visualizar funcionarios");
        ui_center_text("Funcionario nao encontrado.");
        ui_section_title("Pressione <ENTER> para voltar");
    }

    getchar();
    limparTela();
}
