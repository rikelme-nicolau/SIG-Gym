#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"
#include "arquivoPlano.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Permite editar campos de um plano existente e salva no arquivo apos cada mudanca. */

#define P_COL_ID 8
#define P_COL_NOME 24
#define P_COL_HORARIO 10
#define P_COL_VALOR 10
#define P_COL_STATUS 8

static void cabecalho_atualizar(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Entrada de linha simples para evitar problemas de buffer. */
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

/* Mensagem curta usada em varios pontos. */
static void mensagem(const char *sub, const char *linha1)
{
    cabecalho_atualizar(sub);
    ui_text_line(linha1);
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
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

/* Linha de tabela com horario, valor e status alinhados. */
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

/* Fluxo principal: escolhe plano ativo e permite alterar nome, horario, atividades e valor. */
void telaAtualizarPlano(void)
{
    if (total_planos == 0)
    {
        mensagem("Atualizar plano", "Nenhum plano cadastrado.");
        return;
    }

    cabecalho_atualizar("Atualizar plano");
    ui_text_line("Selecione o plano ativo pelo ID.");
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
        mensagem("Atualizar plano", "Nenhum plano ativo.");
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do plano que deseja atualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[12];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    int encontrado = -1;
    for (int i = 0; i < total_planos; i++)
    {
        if (strcmp(lista_planos[i].id, id_busca) == 0 && lista_planos[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        mensagem("Atualizar plano", "Plano nao encontrado ou inativo.");
        return;
    }

    struct plano *plano_sel = &lista_planos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        cabecalho_atualizar("Atualizar plano");
        char linha_sel[UI_INNER + 1];
        snprintf(linha_sel, sizeof(linha_sel), "Plano: %s (%s)", plano_sel->nome, plano_sel->id);
        ui_text_line(linha_sel);
        ui_empty_line();
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Horario");
        ui_menu_option('3', "Atividades");
        ui_menu_option('4', "Valor");
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        printf(">>> ");
        fflush(stdout);
        opcao = lerTecla();

        switch (opcao)
        {
        case '1':
            cabecalho_atualizar("Atualizar plano");
            ui_text_line("Digite o novo nome:");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(plano_sel->nome, buffer);
            atualizarPlanoNoArquivo(*plano_sel);
            mensagem("Atualizar plano", "Nome atualizado com sucesso.");
            break;

        case '2':
            cabecalho_atualizar("Atualizar plano");
            ui_text_line("Novo horario de inicio (ex: 08:00):");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(plano_sel->horario_inicio, buffer);

            cabecalho_atualizar("Atualizar plano");
            ui_text_line("Novo horario de fim (ex: 20:00):");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(plano_sel->horario_fim, buffer);

            atualizarPlanoNoArquivo(*plano_sel);
            mensagem("Atualizar plano", "Horario atualizado com sucesso.");
            break;

        case '3':
            cabecalho_atualizar("Atualizar plano");
            ui_text_line("Informe as atividades (ENTER para finalizar).");
            ui_line('-');
            plano_sel->total_atividades = 0;
            for (int i = 0; i < MAX_ATIVIDADES; i++)
            {
                char titulo[UI_INNER + 1];
                snprintf(titulo, sizeof(titulo), "Atividade #%d:", i + 1);
                ui_text_line(titulo);
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (strlen(buffer) == 0)
                    break;
                strcpy(plano_sel->atividades[plano_sel->total_atividades], buffer);
                plano_sel->total_atividades++;
            }
            atualizarPlanoNoArquivo(*plano_sel);
            mensagem("Atualizar plano", "Atividades atualizadas com sucesso.");
            break;

        case '4':
            cabecalho_atualizar("Atualizar plano");
            ui_text_line("Digite o novo valor da mensalidade:");
            ui_line('=');
            while (true)
            {
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;

                char *endptr = NULL;
                double valor = strtod(buffer, &endptr);
                if (endptr == buffer || *endptr != '\0' || valor <= 0)
                {
                    cabecalho_atualizar("Atualizar plano");
                    ui_text_line("Valor invalido. Use numero maior que zero (ex: 149.90).");
                    ui_section_title("Pressione <ENTER> para tentar novamente");
                    getchar();
                    cabecalho_atualizar("Atualizar plano");
                    ui_text_line("Digite o novo valor da mensalidade:");
                    ui_line('=');
                    continue;
                }

                plano_sel->valor = valor;
                atualizarPlanoNoArquivo(*plano_sel);
                mensagem("Atualizar plano", "Valor atualizado com sucesso.");
                break;
            }
            break;

        case '0':
            break;

        default:
            mensagem("Atualizar plano", "Opcao invalida. Use apenas o menu.");
            break;
        }

    } while (opcao != '0');

    limparTela();
}
