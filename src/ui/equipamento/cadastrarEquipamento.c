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

/* Funcao auxiliar para calcular data + 60 dias. */
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

/* Rodape padrao para entradas, com dica de cancelamento. */
static void rodape_prompt(const char *msg)
{
    ui_line('-');
    ui_text_line(msg);
    ui_text_line(">>> Digite abaixo e pressione ENTER (0 para cancelar).");
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

/* Permite encerrar cadastro digitando 0 em qualquer passo. */
static bool deseja_cancelar(const char *entrada)
{
    return strcmp(entrada, "0") == 0;
}

/* Tela de aviso quando usuario opta por cancelar. */
static void mostrar_cancelamento(void)
{
    cabecalho_equip("Cadastro cancelado");
    ui_center_text("Operacao interrompida pelo usuario.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

// Valida formato DD/MM/AAAA e evita datas futuras
static bool validarData(const char *data)
{
    if (data == NULL || strlen(data) != 10)
    {
        return false;
    }
    if (data[2] != '/' || data[5] != '/')
    {
        return false;
    }

    int dia, mes, ano;
    if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
    {
        return false;
    }

    if (ano < 1900 || mes < 1 || mes > 12)
    {
        return false;
    }

    int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
    {
        dias_no_mes[2] = 29;
    }
    if (dia < 1 || dia > dias_no_mes[mes])
    {
        return false;
    }

    time_t agora = time(NULL);
    struct tm *hoje = localtime(&agora);
    int ano_atual = hoje->tm_year + 1900;
    int mes_atual = hoje->tm_mon + 1;
    int dia_atual = hoje->tm_mday;

    if (ano > ano_atual ||
        (ano == ano_atual && mes > mes_atual) ||
        (ano == ano_atual && mes == mes_atual && dia > dia_atual))
    {
        return false;
    }

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

    /* Captura do nome com opcao de cancelar. */
    ui_text_line("Nome do equipamento.");
    rodape_prompt("Nome:");
    if (!ler_linha(novo.nome, sizeof(novo.nome)))
        return;
    if (deseja_cancelar(novo.nome))
    {
        mostrar_cancelamento();
        return;
    }

    cabecalho_equip("Cadastrar equipamento");
    ui_text_line("Data da ultima manutencao (dd/mm/aaaa).");
    rodape_prompt("Data da ultima manutencao:");
    while (true)
    {
        if (!ler_linha(novo.ultima_manutencao, sizeof(novo.ultima_manutencao)))
            return;

        if (deseja_cancelar(novo.ultima_manutencao))
        {
            mostrar_cancelamento();
            return;
        }

        if (validarData(novo.ultima_manutencao))
        {
            break;
        }

        cabecalho_equip("Cadastrar equipamento");
        ui_text_line("Data invalida. Use DD/MM/AAAA e nao use uma data futura.");
        ui_section_title("Pressione <ENTER> para tentar novamente");
        getchar();
        cabecalho_equip("Cadastrar equipamento");
        ui_text_line("Data da ultima manutencao (dd/mm/aaaa).");
        rodape_prompt("Data da ultima manutencao:");
    }

    calcularProximaManutencao(novo.ultima_manutencao, novo.proxima_manutencao);

    cabecalho_equip("Cadastrar equipamento");
    ui_text_line("Categoria (grupo muscular principal).");
    rodape_prompt("Categoria:");
    if (!ler_linha(novo.categoria, sizeof(novo.categoria)))
        return;
    if (deseja_cancelar(novo.categoria))
    {
        mostrar_cancelamento();
        return;
    }

    novo.ativo = true;
    lista_equipamentos[total_equipamentos++] = novo;
    salvarEquipamentos(lista_equipamentos, total_equipamentos);

    cabecalho_equip("Cadastrar equipamento");
    ui_center_text("Equipamento cadastrado com sucesso.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
