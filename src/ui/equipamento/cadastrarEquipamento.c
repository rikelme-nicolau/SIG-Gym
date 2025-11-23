#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"
#include "ui/utils/consoleLayout.h"

struct equipamento lista_equipamentos[MAX_EQUIPAMENTOS];
int total_equipamentos = 0;

// Função auxiliar para calcular data + 60 dias
void calcularProximaManutencao(const char *data, char *saida)
{
    struct tm tm_data = {0};
    if (strptime(data, "%d/%m/%Y", &tm_data) == NULL)
    {
        strcpy(saida, "Invalida");
        return;
    }

    tm_data.tm_mday += 60;
    mktime(&tm_data);

    strftime(saida, 11, "%d/%m/%Y", &tm_data);
}

static void cabecalho_equip(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

static void rodape_prompt(const char *msg)
{
    ui_line('-');
    ui_text_line(msg);
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
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

void telaCadastrarEquipamento(void)
{
    cabecalho_equip("Cadastrar equipamento");

    if (total_equipamentos >= MAX_EQUIPAMENTOS)
    {
        ui_center_text("Limite maximo de equipamentos alcancado.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    struct equipamento novo = {0};
    snprintf(novo.id, sizeof(novo.id), "%d", total_equipamentos + 1);

    ui_text_line("Nome do equipamento.");
    rodape_prompt("Nome:");
    if (!ler_linha(novo.nome, sizeof(novo.nome)))
        return;

    cabecalho_equip("Cadastrar equipamento");
    ui_text_line("Data da ultima manutencao (dd/mm/aaaa).");
    rodape_prompt("Data da ultima manutencao:");
    if (!ler_linha(novo.ultima_manutencao, sizeof(novo.ultima_manutencao)))
        return;

    calcularProximaManutencao(novo.ultima_manutencao, novo.proxima_manutencao);

    cabecalho_equip("Cadastrar equipamento");
    ui_text_line("Categoria (grupo muscular principal).");
    rodape_prompt("Categoria:");
    if (!ler_linha(novo.categoria, sizeof(novo.categoria)))
        return;

    novo.ativo = true;
    lista_equipamentos[total_equipamentos++] = novo;
    salvarEquipamentos(lista_equipamentos, total_equipamentos);

    cabecalho_equip("Cadastrar equipamento");
    ui_center_text("Equipamento cadastrado com sucesso.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
