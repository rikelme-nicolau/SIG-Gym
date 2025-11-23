#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

#define EQ_COL_ID 8
#define EQ_COL_NOME 22
#define EQ_COL_CATEG 14
#define EQ_COL_DATA 10
#define EQ_COL_STATUS 6
#define MAX_BUFFER 1024

static void cabecalho_atualizar(const char *subtitulo)
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

static void mensagem(const char *sub, const char *texto)
{
    cabecalho_atualizar(sub);
    ui_text_line(texto);
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

void telaAtualizarEquipamento(void)
{
    if (total_equipamentos == 0)
    {
        mensagem("Atualizar equipamento", "Nenhum equipamento cadastrado.");
        return;
    }

    cabecalho_atualizar("Atualizar equipamento");
    ui_text_line("Selecione o equipamento ativo pelo ID.");
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
        mensagem("Atualizar equipamento", "Nenhum equipamento ativo.");
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do equipamento que deseja atualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[12];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

    int encontrado = -1;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (strcmp(lista_equipamentos[i].id, id_busca) == 0 && lista_equipamentos[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        mensagem("Atualizar equipamento", "ID nao encontrado ou inativo.");
        return;
    }

    struct equipamento *equip_sel = &lista_equipamentos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        cabecalho_atualizar("Atualizar equipamento");
        char linha_sel[UI_INNER + 1];
        snprintf(linha_sel, sizeof(linha_sel), "Equipamento: %s (%s)", equip_sel->nome, equip_sel->id);
        ui_text_line(linha_sel);
        ui_empty_line();
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Ultima manutencao");
        ui_menu_option('3', "Categoria");
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        printf(">>> ");
        fflush(stdout);
        opcao = lerTecla();

        switch (opcao)
        {
        case '1':
            cabecalho_atualizar("Atualizar equipamento");
            ui_text_line("Digite o novo nome:");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(equip_sel->nome, buffer);
            atualizarEquipamentoNoArquivo(*equip_sel);
            mensagem("Atualizar equipamento", "Nome atualizado com sucesso.");
            break;

        case '2':
            cabecalho_atualizar("Atualizar equipamento");
            ui_text_line("Nova data da ultima manutencao (dd/mm/aaaa):");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(equip_sel->ultima_manutencao, buffer);
            calcularProximaManutencao(equip_sel->ultima_manutencao, equip_sel->proxima_manutencao);
            atualizarEquipamentoNoArquivo(*equip_sel);
            mensagem("Atualizar equipamento", "Manutencao atualizada com sucesso.");
            break;

        case '3':
            cabecalho_atualizar("Atualizar equipamento");
            ui_text_line("Digite a nova categoria:");
            ui_line('=');
            if (!ler_linha(buffer, sizeof(buffer)))
                break;
            strcpy(equip_sel->categoria, buffer);
            atualizarEquipamentoNoArquivo(*equip_sel);
            mensagem("Atualizar equipamento", "Categoria atualizada com sucesso.");
            break;

        case '0':
            break;

        default:
            mensagem("Atualizar equipamento", "Opcao invalida. Use apenas o menu.");
            break;
        }

    } while (opcao != '0');

    limparTela();
}
