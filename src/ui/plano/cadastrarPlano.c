#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"

#include "cadastrarPlano.h"
#include "arquivoPlano.h" // persistencia
#include "ui/utils/consoleLayout.h"

struct plano lista_planos[MAX_PLANOS];
int total_planos = 0;

/* Tela de cadastro de plano: coleta campos principais, valida valores
   basicos e grava no arquivo binario. */
static void cabecalho_plano(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Mensagem repetida nos prompts para padronizar a entrada. */
static void rodape_prompt(const char *msg)
{
    ui_line('-');
    ui_text_line(msg);
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
}

/* Leitura simples de linha, garantindo string terminada em '\0'. */
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

/* Fluxo principal de cadastro com passos sequenciais para cada campo. */
void telaCadastrarPlano(void)
{
    if (total_planos >= MAX_PLANOS)
    {
        cabecalho_plano("Cadastrar plano");
        ui_center_text("Limite de planos atingido.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    struct plano novo_plano = {0};
    char buffer[MAX_BUFFER];

    cabecalho_plano("Cadastrar plano");
    ui_text_line("Informe o nome do plano.");
    rodape_prompt("Nome do plano:");
    if (!ler_linha(buffer, sizeof(buffer)))
        return;
    strcpy(novo_plano.nome, buffer);

    cabecalho_plano("Cadastrar plano");
    ui_text_line("Horario de inicio (ex: 08:00).");
    rodape_prompt("Horario de inicio:");
    if (!ler_linha(buffer, sizeof(buffer)))
        return;
    strcpy(novo_plano.horario_inicio, buffer);

    cabecalho_plano("Cadastrar plano");
    ui_text_line("Horario de fim (ex: 20:00).");
    rodape_prompt("Horario de fim:");
    if (!ler_linha(buffer, sizeof(buffer)))
        return;
    strcpy(novo_plano.horario_fim, buffer);

    novo_plano.total_atividades = 0;
    for (int i = 0; i < MAX_ATIVIDADES; i++)
    {
        cabecalho_plano("Cadastrar plano");
        char titulo[UI_INNER + 1];
        snprintf(titulo, sizeof(titulo), "Atividade #%d (ENTER para finalizar):", i + 1);
        ui_text_line(titulo);
        rodape_prompt("Digite a atividade:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (strlen(buffer) == 0)
            break;
        strcpy(novo_plano.atividades[novo_plano.total_atividades], buffer);
        novo_plano.total_atividades++;
    }

    cabecalho_plano("Cadastrar plano");
    ui_text_line("Valor da mensalidade (ex: 149.90).");
    rodape_prompt("Valor:");
    while (true)
    {
        if (!ler_linha(buffer, sizeof(buffer)))
            return;

        char *endptr = NULL;
        double valor = strtod(buffer, &endptr);
        if (endptr == buffer || *endptr != '\0' || valor <= 0)
        {
            cabecalho_plano("Cadastrar plano");
            ui_text_line("Valor invalido. Use numero maior que zero (ex: 149.90).");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
            cabecalho_plano("Cadastrar plano");
            ui_text_line("Valor da mensalidade (ex: 149.90).");
            rodape_prompt("Valor:");
            continue;
        }

        novo_plano.valor = valor;
        break;
    }

    snprintf(novo_plano.id, sizeof(novo_plano.id), "%d", total_planos + 1);
    novo_plano.ativo = true;

    lista_planos[total_planos++] = novo_plano;
    salvarPlanos(lista_planos, total_planos);

    cabecalho_plano("Cadastrar plano");
    ui_center_text("Plano cadastrado com sucesso.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
