#include <stdio.h>
#include <time.h>
#include <string.h>

#include "logGeracao.h"
#include "../aluno/cadastrarAluno.h"
#include "../plano/cadastrarPlano.h"
#include "../funcionario/cadastrarFuncionario.h"
#include "../equipamento/cadastrarEquipamento.h"

#define LOG_ARQUIVO "geracao_dados.log"

static int log_iniciado = 0;

static void escreverLinha(const char *linha)
{
    FILE *fp = fopen(LOG_ARQUIVO, "a");
    if (!fp)
    {
        return;
    }
    fputs(linha, fp);
    fclose(fp);
}

static void linhaTimestamp(char *buffer, size_t tamanho)
{
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(buffer, tamanho, "[%Y-%m-%d %H:%M:%S] ", tm_info);
}

void logInicioGeracao(void)
{
    FILE *fp = fopen(LOG_ARQUIVO, "w");
    if (!fp)
    {
        return;
    }

    char ts[32];
    linhaTimestamp(ts, sizeof(ts));
    fprintf(fp, "%sInicio da geracao de dados\n", ts);
    fclose(fp);
    log_iniciado = 1;
}

void logEtapaGeracao(const char *modulo, int quantidade)
{
    if (!log_iniciado)
    {
        logInicioGeracao();
    }

    char ts[32];
    linhaTimestamp(ts, sizeof(ts));

    char linha[256];
    snprintf(linha, sizeof(linha), "%sGerados %d %s\n", ts, quantidade, modulo ? modulo : "");
    escreverLinha(linha);
}

void logFimGeracao(void)
{
    if (!log_iniciado)
    {
        logInicioGeracao();
    }

    char ts[32];
    linhaTimestamp(ts, sizeof(ts));

    char linha[256];
    snprintf(linha, sizeof(linha), "%sResumo final -> Alunos: %d, Planos: %d, Funcionarios: %d, Equipamentos: %d\n",
             ts, total_alunos, total_planos, total_funcionarios, total_equipamentos);
    escreverLinha(linha);
}
