#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>           // <-- faltava
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "arquivoAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "ui/utils/consoleLayout.h"

/* Tela de exclusao logica: lista alunos ativos, pede um ID e marca como inativo
   tanto no vetor em memoria quanto no arquivo binario. */

// reutilize as mesmas helpers do modulo de funcionarios
static void limparString(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
    for (int i = (int)strlen(str) - 1; i >= 0 && str[i] == ' '; i--)
        str[i] = '\0';
}

/* Entrada segura em string curta, removendo \n e espacos finais. */
static bool lerString(char *dest, int tamanho) {
    if (fgets(dest, tamanho, stdin) == NULL) {
        dest[0] = '\0';
        return false;
    }
    limparString(dest);
    return true;
}

static void cabecalho_excluir(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Cabecalho/linha da tabela para reaproveitar a mesma diagramacao. */
static void tabela_alunos_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", 12);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", 26);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", 17);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", 8);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

/* Linha com dados do aluno a ser exibida na listagem. */
static void tabela_alunos_row(const char *id, const char *nome, const char *plano, const char *status)
{
    char status_clip[16];
    ui_clip_utf8(status, 8, status_clip, sizeof(status_clip));
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, id, 12);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nome, 26);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, plano, 17);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status_clip, 8);
    linha[pos] = '\0';
    ui_text_line(linha);
}

/* Descobre o nome do plano para mostrar na tabela. */
static void preencher_nome_plano(const struct aluno *aluno, char *dest, size_t size)
{
    strncpy(dest, "Sem plano", size - 1);
    dest[size - 1] = '\0';

    if (strcmp(aluno->plano_id, "0") != 0)
    {
        for (int j = 0; j < total_planos; j++)
        {
            if (lista_planos[j].ativo && strcmp(lista_planos[j].id, aluno->plano_id) == 0)
            {
                strncpy(dest, lista_planos[j].nome, size - 1);
                dest[size - 1] = '\0';
                break;
            }
        }
    }
}

/* Fluxo principal: mostra alunos ativos, le ID e marca como inativo. */
void telaExcluirAluno(void)
{
    if (total_alunos == 0) {
        cabecalho_excluir("Excluir aluno");
        ui_center_text("Nenhum aluno cadastrado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    cabecalho_excluir("Excluir aluno");
    ui_text_line("Selecione o aluno ativo para excluir.");
    tabela_alunos_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_alunos; i++) {
        if (lista_alunos[i].ativo) {
            char nome_plano[MAX_BUFFER];
            preencher_nome_plano(&lista_alunos[i], nome_plano, sizeof(nome_plano));
            tabela_alunos_row(lista_alunos[i].id, lista_alunos[i].nome, nome_plano, "Ativo");
            algum_ativo = 1;
        }
    }

    if (!algum_ativo) {
        ui_section_title("Nenhum aluno ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do aluno que deseja excluir.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[32];                 // <-- aumente para evitar truncamento
    if (!lerString(id_busca, sizeof(id_busca))) {
        limparTela();
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        // limpe o ID salvo antes de comparar
        char id_aluno[32];
        strncpy(id_aluno, lista_alunos[i].id, sizeof(id_aluno));
        id_aluno[sizeof(id_aluno)-1] = '\0';
        limparString(id_aluno);

        if (strcmp(id_aluno, id_busca) == 0 && lista_alunos[i].ativo) {
            lista_alunos[i].ativo = false;

            // OPCAO A: manter sua funcao (se ela regrava o arquivo corretamente)
            excluirAluno(id_busca);

            // OPCAO B (alternativa, mais simetrica ao modulo de funcionarios):
            // salvarAlunos(lista_alunos, total_alunos);

            cabecalho_excluir("Excluir aluno");
            ui_center_text("Aluno excluido com sucesso.");
            ui_section_title("Pressione <ENTER> para voltar");
            getchar();
            limparTela();
            return;
        }
    }

    cabecalho_excluir("Excluir aluno");
    ui_center_text("Aluno nao encontrado.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
