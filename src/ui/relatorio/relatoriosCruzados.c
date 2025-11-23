#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "relatoriosCruzados.h"
#include "telaRelatoriosCruzados.h"
#include "opInvalida.h"
#include "limparTela.h"

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "mergeListas.h"
#include "ui/utils/consoleLayout.h"

#define PESS_COL_TIPO 4
#define PESS_COL_ID 8
#define PESS_COL_NOME 20
#define PESS_COL_IDADE 6

#define ALUNO_COL_ID 8
#define ALUNO_COL_NOME 18
#define ALUNO_COL_PLANO 16
#define ALUNO_COL_VALOR 10

#define HORARIO_COL_FAIXA 10
#define HORARIO_COL_PLANO 18
#define HORARIO_COL_ALUNOS 8
#define HORARIO_COL_PCT_FAIXA 8
#define HORARIO_COL_PCT_TOTAL 10

#define EQUIP_COL_ID 10
#define EQUIP_COL_NOME 18
#define EQUIP_COL_ULT 12
#define EQUIP_COL_PROX 12
#define EQUIP_COL_STATUS 10

#define FUNC_COL_ID 8
#define FUNC_COL_NOME 18
#define FUNC_COL_CARGO 16
#define FUNC_COL_IDADE 6
#define FUNC_COL_PLANO 12

#define ATIV_COL_PLANO 20
#define ATIV_COL_ATIV 16
#define ATIV_COL_PCT 8

struct PlanoHorarioView
{
    const struct plano *plano;
    int faixa;
    int alunos;
};

static void relatorioRelacaoAlunoPlano(void);
static void relatorioCruzadoPlanoHorarioAlunos(void);
static void relatorioEquipamentoCategoriaManutencao(void);
static void relatorioCruzadoFuncionarioCargoIdade(void);
static void relatorioAlunoIdadePlano(void);
static void relatorioPlanoAtividadePopularidade(void);
static void relatorioTodasPessoasUnificado(void);
static void exportarPessoasCsv(const struct PessoaView *pessoas, int total, int totalAlunos, int totalFuncs);
static const struct plano *buscarPlanoPorId(const char *id);
static int contarAlunosDoPlano(const char *id);
static int faixaHorarioPlano(const struct plano *plano);
static const char *descricaoFaixa(int faixa);
static double calcularDiasRestantes(const char *data);
static int compararAlunosPorNome(const void *a, const void *b);
static int compararFuncionariosPorNome(const void *a, const void *b);
static void cabecalho_relatorio(const char *subtitulo);
static void aguardar_voltar(void);
static void tabela_pessoas_header(void);
static void tabela_pessoas_row(const struct PessoaView *p);
static void tabela_aluno_plano_header(void);
static void tabela_aluno_plano_row(const struct aluno *al, const struct plano *pl);
static void tabela_horario_header(void);
static void tabela_horario_row(const struct PlanoHorarioView *vw, double pctFaixa, double pctTotal);
static void tabela_equip_header(void);
static void tabela_equip_row(const struct equipamento *eq, const char *status);
static void tabela_func_header(const char *titulo);
static void tabela_func_row(const char *id, const char *nome, const char *cargo, int idade, const char *extra);
static void tabela_ativ_header(void);
static void tabela_ativ_row(const char *plano, const char *ativ, double pct);

static void cabecalho_relatorio(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

static void aguardar_voltar(void)
{
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

static void tabela_pessoas_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Tipo", PESS_COL_TIPO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "ID", PESS_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", PESS_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Idade", PESS_COL_IDADE);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_pessoas_row(const struct PessoaView *p)
{
    if (p == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, p->tipo == 'A' ? "Aluno" : "Func", PESS_COL_TIPO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, p->id, PESS_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, p->nome, PESS_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    char idade[8];
    snprintf(idade, sizeof(idade), "%d", p->idade);
    ui_append_col(linha, sizeof(linha), &pos, idade, PESS_COL_IDADE);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_aluno_plano_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", ALUNO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Aluno", ALUNO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", ALUNO_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Valor", ALUNO_COL_VALOR);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_aluno_plano_row(const struct aluno *al, const struct plano *pl)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char valor[16];
    snprintf(valor, sizeof(valor), "R$ %.2f", pl != NULL ? pl->valor : 0.0);
    ui_append_col(linha, sizeof(linha), &pos, al->id, ALUNO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, al->nome, ALUNO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pl != NULL ? pl->nome : "Sem plano", ALUNO_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, valor, ALUNO_COL_VALOR);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_horario_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Horario", HORARIO_COL_FAIXA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", HORARIO_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", HORARIO_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "% Faixa", HORARIO_COL_PCT_FAIXA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "% Total", HORARIO_COL_PCT_TOTAL);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_horario_row(const struct PlanoHorarioView *vw, double pctFaixa, double pctTotal)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char alunos[12];
    snprintf(alunos, sizeof(alunos), "%d", vw->alunos);
    char pctF[12];
    snprintf(pctF, sizeof(pctF), "%.1f", pctFaixa);
    char pctT[12];
    snprintf(pctT, sizeof(pctT), "%.1f", pctTotal);
    ui_append_col(linha, sizeof(linha), &pos, descricaoFaixa(vw->faixa), HORARIO_COL_FAIXA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, vw->plano->nome, HORARIO_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, alunos, HORARIO_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctF, HORARIO_COL_PCT_FAIXA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctT, HORARIO_COL_PCT_TOTAL);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_equip_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", EQUIP_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Equipamento", EQUIP_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Ultima", EQUIP_COL_ULT);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Proxima", EQUIP_COL_PROX);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", EQUIP_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_equip_row(const struct equipamento *eq, const char *status)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, eq->id, EQUIP_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->nome, EQUIP_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->ultima_manutencao, EQUIP_COL_ULT);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->proxima_manutencao, EQUIP_COL_PROX);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status, EQUIP_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_func_header(const char *titulo)
{
    ui_section_title(titulo);
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", FUNC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", FUNC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Cargo", FUNC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Idade", FUNC_COL_IDADE);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Info", FUNC_COL_PLANO);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_func_row(const char *id, const char *nome, const char *cargo, int idade, const char *extra)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[32];
    ui_clip_utf8(nome, 18, nomeClip, sizeof(nomeClip));
    char cargoClip[24];
    ui_clip_utf8(cargo, 16, cargoClip, sizeof(cargoClip));
    char idadeStr[8];
    snprintf(idadeStr, sizeof(idadeStr), "%d", idade);
    ui_append_col(linha, sizeof(linha), &pos, id, FUNC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nomeClip, FUNC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, cargoClip, FUNC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, idadeStr, FUNC_COL_IDADE);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, extra != NULL ? extra : "", FUNC_COL_PLANO);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_ativ_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Plano", ATIV_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Atividade", ATIV_COL_ATIV);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "% Popular", ATIV_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_ativ_row(const char *plano, const char *ativ, double pct)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char planoClip[32];
    ui_clip_utf8(plano, 20, planoClip, sizeof(planoClip));
    char ativClip[24];
    ui_clip_utf8(ativ, 16, ativClip, sizeof(ativClip));
    char pctStr[12];
    snprintf(pctStr, sizeof(pctStr), "%.1f", pct);
    ui_append_col(linha, sizeof(linha), &pos, planoClip, ATIV_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, ativClip, ATIV_COL_ATIV);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctStr, ATIV_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void relatorioTodasPessoasUnificado(void)
{
    cabecalho_relatorio("Relatorio - Todas as pessoas");

    struct aluno alunosAtivos[MAX_ALUNOS];
    int totalAlunosAtivos = 0;
    for (int i = 0; i < total_alunos && totalAlunosAtivos < MAX_ALUNOS; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunosAtivos[totalAlunosAtivos++] = lista_alunos[i];
        }
    }

    struct funcionario funcsAtivos[MAX_FUNCIONARIOS];
    int totalFuncsAtivos = 0;
    for (int i = 0; i < total_funcionarios && totalFuncsAtivos < MAX_FUNCIONARIOS; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            funcsAtivos[totalFuncsAtivos++] = lista_funcionarios[i];
        }
    }

    if (totalAlunosAtivos == 0 && totalFuncsAtivos == 0)
    {
        ui_center_text("Nao ha alunos ou funcionarios ativos para listar.");
        aguardar_voltar();
        return;
    }

    qsort(alunosAtivos, totalAlunosAtivos, sizeof(struct aluno), compararAlunosPorNome);
    qsort(funcsAtivos, totalFuncsAtivos, sizeof(struct funcionario), compararFuncionariosPorNome);

    int totalPessoas = 0;
    struct PessoaView *pessoas = mergeListasOrdenadas(alunosAtivos, totalAlunosAtivos,
                                                      funcsAtivos, totalFuncsAtivos,
                                                      &totalPessoas);
    if (pessoas == NULL || totalPessoas == 0)
    {
        ui_center_text("Nao foi possivel unificar as listas.");
        free(pessoas);
        aguardar_voltar();
        return;
    }

    int contAlunos = 0;
    int contFuncs = 0;

    tabela_pessoas_header();
    for (int i = 0; i < totalPessoas; i++)
    {
        const struct PessoaView *p = &pessoas[i];
        if (p->tipo == 'A')
        {
            contAlunos++;
        }
        else if (p->tipo == 'F')
        {
            contFuncs++;
        }
        tabela_pessoas_row(p);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total alunos: %d | Total funcionarios: %d | Total geral: %d", contAlunos, contFuncs, totalPessoas);
    ui_text_line(linha);

    ui_section_title("Exportar CSV?");
    ui_text_line("Deseja exportar para CSV? (S/N)");
    ui_line('=');
    printf(">>> ");
    int resp = getchar();
    while (resp == '\n')
    {
        resp = getchar();
    }
    if (resp == 's' || resp == 'S')
    {
        exportarPessoasCsv(pessoas, totalPessoas, contAlunos, contFuncs);
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        // limpa buffer
    }

    free(pessoas);

    aguardar_voltar();
}

static void exportarPessoasCsv(const struct PessoaView *pessoas, int total, int totalAlunos, int totalFuncs)
{
    FILE *fp = fopen("relatorio_pessoas_unificado.csv", "w");
    if (fp == NULL)
    {
        printf("Nao foi possivel criar o arquivo CSV.\n");
        return;
    }

    fprintf(fp, "# Relatorio unificado de pessoas\n");
    fprintf(fp, "# Total alunos;%d\n", totalAlunos);
    fprintf(fp, "# Total funcionarios;%d\n", totalFuncs);
    fprintf(fp, "# Total geral;%d\n", total);
    fprintf(fp, "Tipo;ID;Nome;Idade\n");
    for (int i = 0; i < total; i++)
    {
        fprintf(fp, "%c;%s;%s;%d\n",
                pessoas[i].tipo,
                pessoas[i].id,
                pessoas[i].nome,
                pessoas[i].idade);
    }

    fclose(fp);
    ui_text_line("Arquivo CSV gerado: relatorio_pessoas_unificado.csv");
}

void moduloRelatoriosCruzados(void)
{
    char op;

    do
    {
        op = telaRelatoriosCruzados();

        switch (op)
        {
        case '1':
            relatorioRelacaoAlunoPlano();
            break;
        case '2':
            relatorioCruzadoPlanoHorarioAlunos();
            break;
        case '3':
            relatorioEquipamentoCategoriaManutencao();
            break;
        case '4':
            relatorioCruzadoFuncionarioCargoIdade();
            break;
        case '5':
            relatorioAlunoIdadePlano();
            break;
        case '6':
            relatorioPlanoAtividadePopularidade();
            break;
        case '7':
            relatorioTodasPessoasUnificado();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

static void relatorioRelacaoAlunoPlano(void)
{
    cabecalho_relatorio("Relacao aluno x plano");

    tabela_aluno_plano_header();
    int total_relacoes = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        const struct plano *plano = NULL;
        if (lista_alunos[i].plano_id[0] != '\0' && strcmp(lista_alunos[i].plano_id, "0") != 0)
        {
            plano = buscarPlanoPorId(lista_alunos[i].plano_id);
        }

        tabela_aluno_plano_row(&lista_alunos[i], plano);
        total_relacoes++;
    }

    if (total_relacoes == 0)
    {
        ui_text_line("Nenhuma relacao aluno/plano encontrada.");
    }

    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de registros: %d", total_relacoes);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

static void relatorioCruzadoPlanoHorarioAlunos(void)
{
    cabecalho_relatorio("Plano x horario x alunos");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct PlanoHorarioView views[MAX_PLANOS];
    int totalViews = 0;
    int alunosTotais = 0;
    int alunosPorFaixa[4] = {0};

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int alunos = contarAlunosDoPlano(lista_planos[i].id);
        int faixa = faixaHorarioPlano(&lista_planos[i]);

        views[totalViews].plano = &lista_planos[i];
        views[totalViews].faixa = faixa;
        views[totalViews].alunos = alunos;
        totalViews++;

        alunosTotais += alunos;
        if (faixa >= 0 && faixa < 4)
        {
            alunosPorFaixa[faixa] += alunos;
        }
    }

    if (totalViews == 0 || alunosTotais == 0)
    {
        ui_center_text("Nao ha dados suficientes para gerar este relatorio.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalViews - 1; i++)
    {
        for (int j = i + 1; j < totalViews; j++)
        {
            if (views[j].faixa < views[i].faixa ||
                (views[j].faixa == views[i].faixa &&
                 strcmp(views[j].plano->nome, views[i].plano->nome) < 0))
            {
                struct PlanoHorarioView tmp = views[i];
                views[i] = views[j];
                views[j] = tmp;
            }
        }
    }

    tabela_horario_header();

    double maiorCarga = 0.0;
    double menorCarga = 100.0;
    int faixaMaior = -1;
    int faixaMenor = -1;

    for (int faixa = 0; faixa < 4; faixa++)
    {
        double carga = alunosTotais > 0 ? ((double)alunosPorFaixa[faixa] / alunosTotais) * 100.0 : 0.0;
        if (carga > maiorCarga)
        {
            maiorCarga = carga;
            faixaMaior = faixa;
        }
        if (carga < menorCarga)
        {
            menorCarga = carga;
            faixaMenor = faixa;
        }
    }

    for (int i = 0; i < totalViews; i++)
    {
        double percHora = alunosPorFaixa[views[i].faixa] > 0 ? ((double)views[i].alunos / alunosPorFaixa[views[i].faixa]) * 100.0 : 0.0;
        double percTotal = (double)views[i].alunos / alunosTotais * 100.0;
        tabela_horario_row(&views[i], percHora, percTotal);
    }

    ui_line('-');
    char linha[UI_INNER + 1];
    if (faixaMaior != -1)
    {
        snprintf(linha, sizeof(linha), "Faixa com maior carga: %s (%.1f%%)", descricaoFaixa(faixaMaior), maiorCarga);
        ui_text_line(linha);
    }
    if (faixaMenor != -1)
    {
        snprintf(linha, sizeof(linha), "Faixa com menor carga: %s (%.1f%%)", descricaoFaixa(faixaMenor), menorCarga);
        ui_text_line(linha);
    }
    if (maiorCarga > 60.0)
    {
        snprintf(linha, sizeof(linha), "Atencao: faixa %s pode estar sobrecarregada.", descricaoFaixa(faixaMaior));
        ui_text_line(linha);
    }
    if (menorCarga < 15.0)
    {
        snprintf(linha, sizeof(linha), "Observacao: faixa %s com baixa ocupacao.", descricaoFaixa(faixaMenor));
        ui_text_line(linha);
    }

    snprintf(linha, sizeof(linha), "Total de alunos considerados: %d", alunosTotais);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

static void relatorioEquipamentoCategoriaManutencao(void)
{
    cabecalho_relatorio("Equipamento x categoria x manutencao");

    if (total_equipamentos == 0)
    {
        ui_center_text("Nao ha equipamentos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct CategoriaEquip
    {
        char nome[128];
        const struct equipamento *lista[MAX_EQUIPAMENTOS];
        double dias[MAX_EQUIPAMENTOS];
        int total;
        int emDia;
        int proximas;
        int vencidas;
    };

    // Mantemos o buffer das categorias em armazenamento estatico para evitar
    // estouro de stack (cada categoria carrega buffers grandes).
    static struct CategoriaEquip categorias[MAX_EQUIPAMENTOS];
    memset(categorias, 0, sizeof(categorias));
    int totalCategorias = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        char nomeCat[128];
        size_t len = strlen(lista_equipamentos[i].categoria);
        if (len >= sizeof(nomeCat))
        {
            len = sizeof(nomeCat) - 1;
        }
        for (size_t c = 0; c < len; c++)
        {
            nomeCat[c] = (char)toupper((unsigned char)lista_equipamentos[i].categoria[c]);
        }
        nomeCat[len] = '\0';

        int idx = -1;
        for (int c = 0; c < totalCategorias; c++)
        {
            if (strcmp(categorias[c].nome, nomeCat) == 0)
            {
                idx = c;
                break;
            }
        }

        if (idx == -1 && totalCategorias < MAX_EQUIPAMENTOS)
        {
            strncpy(categorias[totalCategorias].nome, nomeCat, sizeof(categorias[totalCategorias].nome));
            categorias[totalCategorias].nome[sizeof(categorias[totalCategorias].nome) - 1] = '\0';
            categorias[totalCategorias].total = 0;
            categorias[totalCategorias].emDia = 0;
            categorias[totalCategorias].proximas = 0;
            categorias[totalCategorias].vencidas = 0;
            idx = totalCategorias++;
        }

        double dias = calcularDiasRestantes(lista_equipamentos[i].proxima_manutencao);
        const struct equipamento *eq = &lista_equipamentos[i];

        if (idx != -1 && categorias[idx].total < MAX_EQUIPAMENTOS)
        {
            categorias[idx].lista[categorias[idx].total] = eq;
            categorias[idx].dias[categorias[idx].total] = dias;
            categorias[idx].total++;

            if (dias < 0)
            {
                categorias[idx].vencidas++;
            }
            else if (dias <= 7)
            {
                categorias[idx].proximas++;
            }
            else
            {
                categorias[idx].emDia++;
            }
        }
    }

    for (int i = 0; i < totalCategorias; i++)
    {
        ui_section_title(categorias[i].nome);
        tabela_equip_header();

        for (int j = 0; j < categorias[i].total; j++)
        {
            const struct equipamento *eq = categorias[i].lista[j];
            double dias = categorias[i].dias[j];
            const char *status = "Em dia";
            if (dias < 0)
            {
                status = "Vencida";
            }
            else if (dias <= 7)
            {
                status = "Proxima";
            }

            tabela_equip_row(eq, status);
        }

        char linha[UI_INNER + 1];
        ui_line('-');
        snprintf(linha, sizeof(linha), "Subtotal: %d equipamento%s | %d em dia, %d proximas, %d vencidas",
                 categorias[i].total,
                 categorias[i].total == 1 ? "" : "s",
                 categorias[i].emDia,
                 categorias[i].proximas,
                 categorias[i].vencidas);
        ui_text_line(linha);
    }

    ui_line('-');
    aguardar_voltar();
}

static void relatorioCruzadoFuncionarioCargoIdade(void)
{
    cabecalho_relatorio("Funcionario x cargo x faixa etaria");

    if (total_funcionarios == 0)
    {
        ui_center_text("Nao ha funcionarios cadastrados.");
        aguardar_voltar();
        return;
    }

    struct FaixaCargo
    {
        const struct funcionario *func;
        int faixa;
    };

    struct FaixaCargo lista[MAX_FUNCIONARIOS];
    int total = 0;
    int faixas[5] = {0};

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (!lista_funcionarios[i].ativo)
        {
            continue;
        }
        int idade = lista_funcionarios[i].idade;
        int faixa = 4;
        if (idade >= 18 && idade <= 25)
            faixa = 0;
        else if (idade <= 35)
            faixa = 1;
        else if (idade <= 45)
            faixa = 2;
        else if (idade <= 55)
            faixa = 3;

        lista[total].func = &lista_funcionarios[i];
        lista[total].faixa = faixa;
        total++;
        faixas[faixa]++;
    }

    if (total == 0)
    {
        ui_center_text("Nenhum funcionario ativo para exibir.");
        aguardar_voltar();
        return;
    }

    tabela_func_header("Funcionarios por faixa");
    for (int i = 0; i < total; i++)
    {
        const char *faixaDesc = descricaoFaixa(lista[i].faixa);
        tabela_func_row(lista[i].func->id, lista[i].func->nome, lista[i].func->cargo, lista[i].func->idade, faixaDesc);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    for (int f = 0; f < 4; f++)
    {
        double pct = (double)faixas[f] / total * 100.0;
        snprintf(linha, sizeof(linha), "%s: %d funcionario(s) - %.1f%%", descricaoFaixa(f), faixas[f], pct);
        ui_text_line(linha);
    }
    ui_line('-');
    aguardar_voltar();
}

static void relatorioAlunoIdadePlano(void)
{
    cabecalho_relatorio("Aluno x idade x plano");

    if (total_alunos == 0)
    {
        ui_center_text("Nao ha alunos cadastrados.");
        aguardar_voltar();
        return;
    }

    tabela_aluno_plano_header();
    int total = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }
        const struct plano *pl = buscarPlanoPorId(lista_alunos[i].plano_id);
        tabela_aluno_plano_row(&lista_alunos[i], pl);
        total++;
    }

    if (total == 0)
    {
        ui_text_line("Nenhum aluno ativo para exibir.");
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total exibido: %d", total);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

static void relatorioPlanoAtividadePopularidade(void)
{
    cabecalho_relatorio("Plano x atividade x popularidade");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct AtivView
    {
        const struct plano *plano;
        char atividade[64];
        int alunos;
    };

    struct AtivView lista[MAX_PLANOS * MAX_ATIVIDADES];
    int total = 0;
    int alunosTotais = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }
        int alunosPlano = contarAlunosDoPlano(lista_planos[i].id);
        alunosTotais += alunosPlano;

        for (int j = 0; j < lista_planos[i].total_atividades; j++)
        {
            if (lista_planos[i].atividades[j][0] == '\0')
            {
                continue;
            }
            if (total < MAX_PLANOS * MAX_ATIVIDADES)
            {
                lista[total].plano = &lista_planos[i];
                strncpy(lista[total].atividade, lista_planos[i].atividades[j], sizeof(lista[total].atividade));
                lista[total].atividade[sizeof(lista[total].atividade) - 1] = '\0';
                lista[total].alunos = alunosPlano;
                total++;
            }
        }
    }

    if (total == 0 || alunosTotais == 0)
    {
        ui_center_text("Nao ha atividades cadastradas nos planos.");
        aguardar_voltar();
        return;
    }

    tabela_ativ_header();
    for (int i = 0; i < total; i++)
    {
        double pct = alunosTotais > 0 ? ((double)lista[i].alunos / alunosTotais) * 100.0 : 0.0;
        tabela_ativ_row(lista[i].plano->nome, lista[i].atividade, pct);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de alunos considerados: %d", alunosTotais);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

static const struct plano *buscarPlanoPorId(const char *id)
{
    if (id[0] == '\0')
    {
        return NULL;
    }

    for (int i = 0; i < total_planos; i++)
    {
        if (strcmp(lista_planos[i].id, id) == 0)
        {
            return &lista_planos[i];
        }
    }

    return NULL;
}

static int contarAlunosDoPlano(const char *id)
{
    if (id == NULL || id[0] == '\0')
    {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }
        if (strcmp(lista_alunos[i].plano_id, id) == 0)
        {
            total++;
        }
    }
    return total;
}

static int faixaHorarioPlano(const struct plano *plano)
{
    if (plano == NULL || plano->horario_inicio[0] == '\0')
    {
        return 3;
    }

    int hora = 0;
    if (sscanf(plano->horario_inicio, "%2d", &hora) != 1)
    {
        return 3;
    }

    if (hora >= 5 && hora < 12)
    {
        return 0;
    }
    if (hora >= 12 && hora < 18)
    {
        return 1;
    }
    if (hora >= 18 && hora < 24)
    {
        return 2;
    }
    return 3;
}

static const char *descricaoFaixa(int faixa)
{
    switch (faixa)
    {
    case 0:
        return "06-12h";
    case 1:
        return "12-18h";
    case 2:
        return "18-22h";
    default:
        return "Outros";
    }
}

static double calcularDiasRestantes(const char *data)
{
    if (data == NULL || data[0] == '\0')
    {
        return 0.0;
    }

    int dia = 0, mes = 0, ano = 0;
    if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
    {
        return 0.0;
    }

    struct tm alvo = {0};
    alvo.tm_mday = dia;
    alvo.tm_mon = mes - 1;
    alvo.tm_year = ano - 1900;

    time_t tAlvo = mktime(&alvo);
    time_t agora = time(NULL);

    return difftime(tAlvo, agora) / (60 * 60 * 24);
}

static int compararAlunosPorNome(const void *a, const void *b)
{
    const struct aluno *al = (const struct aluno *)a;
    const struct aluno *bl = (const struct aluno *)b;
    return strcasecmp(al->nome, bl->nome);
}

static int compararFuncionariosPorNome(const void *a, const void *b)
{
    const struct funcionario *fa = (const struct funcionario *)a;
    const struct funcionario *fb = (const struct funcionario *)b;
    return strcasecmp(fa->nome, fb->nome);
}
