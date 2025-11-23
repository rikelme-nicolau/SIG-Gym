#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosPlano.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/aluno/cadastrarAluno.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

#define PLAN_COL_ID 8
#define PLAN_COL_NOME 18
#define PLAN_COL_STATUS 7
#define PLAN_COL_VALOR 8
#define PLAN_COL_ALUNOS 6
#define PLAN_COL_RECEITA 10

#define RANK_COL_POS 4
#define RANK_COL_NOME 20
#define RANK_COL_ALUNOS 8
#define RANK_COL_PCT 8

#define REC_COL_PLANO 20
#define REC_COL_ALUNOS 8
#define REC_COL_MENSAL 12
#define REC_COL_ANUAL 12
#define REC_COL_PCT 8

#define ATIV_COL_NOME 24
#define ATIV_COL_PLANOS 8
#define ATIV_COL_ALUNOS 10
#define ATIV_COL_PCT 8

#define TOP_COL_RANK 4
#define TOP_COL_NOME 16
#define TOP_COL_ALUNOS 8
#define TOP_COL_REC_M 10
#define TOP_COL_REC_A 10
#define TOP_COL_PCT 8

static void relatorioListagemPlanos(void);
static void relatorioRankingPopularidade(void);
static void relatorioReceitaPorPlano(void);
static void relatorioDistribuicaoAtividades(void);
static void relatorioOcupacaoHorarioPlano(void);
static void relatorioPlanosMaisLucrativos(void);

struct PlanoView
{
    const struct plano *plano;
    int alunosVinculados;
    double receita;
};

static char selecionarStatusPlano(void);
static void solicitarTermoPlanos(char *dest, size_t tamanho);
static char selecionarOrdenacaoPlanos(void);
static bool planoCorrespondeFiltros(const struct plano *plano, char statusFiltro, const char *termo);
static bool statusPlanoCorresponde(bool ativo, char statusFiltro);
static bool textoContemPlano(const char *texto, const char *busca);
static void ordenarPlanos(struct PlanoView *lista, int total, char criterio);
static int compararPlanosView(const struct PlanoView *a, const struct PlanoView *b, char criterio);
static int compararStringsInsensitive(const char *a, const char *b);
static int contarAlunosNoPlano(const struct plano *plano);
static void quicksortPlanosPorReceita(struct PlanoView *lista, int inicio, int fim);
static void cabecalho_relatorio(const char *subtitulo);
static void aguardar_voltar(void);
static void tabela_planos_header(void);
static void tabela_planos_row(const struct PlanoView *view);
static void tabela_rank_header(void);
static void tabela_rank_row(int pos, const struct PlanoView *view, double percentual);
static void tabela_receita_header(void);
static void tabela_receita_row(const struct PlanoView *view, double pct);
static void tabela_atividades_header(void);
static void tabela_atividades_row(const char *nome, int planos, int alunos, double pct);

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

static void tabela_planos_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", PLAN_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", PLAN_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", PLAN_COL_STATUS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Valor", PLAN_COL_VALOR);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", PLAN_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Receita", PLAN_COL_RECEITA);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_planos_row(const struct PlanoView *view)
{
    if (view == NULL || view->plano == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[32];
    ui_clip_utf8(view->plano->nome, 18, nomeClip, sizeof(nomeClip));
    char status[8];
    strncpy(status, view->plano->ativo ? "Ativo" : "Inativo", sizeof(status));
    status[sizeof(status) - 1] = '\0';
    char valor[16];
    snprintf(valor, sizeof(valor), "R$%.2f", view->plano->valor);
    char alunos[12];
    snprintf(alunos, sizeof(alunos), "%d", view->alunosVinculados);
    char receita[16];
    snprintf(receita, sizeof(receita), "R$%.2f", view->receita);

    ui_append_col(linha, sizeof(linha), &pos, view->plano->id, PLAN_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nomeClip, PLAN_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status, PLAN_COL_STATUS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, valor, PLAN_COL_VALOR);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, alunos, PLAN_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, receita, PLAN_COL_RECEITA);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_rank_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Pos", RANK_COL_POS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", RANK_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", RANK_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "%", RANK_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_rank_row(int posRank, const struct PlanoView *view, double percentual)
{
    if (view == NULL || view->plano == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    char rankStr[8];
    snprintf(rankStr, sizeof(rankStr), "%dº", posRank);
    char nomeClip[32];
    ui_clip_utf8(view->plano->nome, 20, nomeClip, sizeof(nomeClip));
    char alunosStr[16];
    snprintf(alunosStr, sizeof(alunosStr), "%d", view->alunosVinculados);
    char pctStr[16];
    snprintf(pctStr, sizeof(pctStr), "%.2f", percentual);

    ui_append_col(linha, sizeof(linha), &pos, rankStr, RANK_COL_POS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nomeClip, RANK_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, alunosStr, RANK_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctStr, RANK_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_receita_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Plano", REC_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", REC_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Rec. Mensal", REC_COL_MENSAL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Rec. Anual", REC_COL_ANUAL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "% Receita", REC_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_receita_row(const struct PlanoView *view, double pct)
{
    if (view == NULL || view->plano == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[40];
    ui_clip_utf8(view->plano->nome, 20, nomeClip, sizeof(nomeClip));
    char alunosStr[16];
    snprintf(alunosStr, sizeof(alunosStr), "%d", view->alunosVinculados);
    char recMensal[24];
    snprintf(recMensal, sizeof(recMensal), "R$ %.2f", view->receita);
    char recAnual[24];
    snprintf(recAnual, sizeof(recAnual), "R$ %.2f", view->receita * 12.0);
    char pctStr[16];
    snprintf(pctStr, sizeof(pctStr), "%.2f", pct);

    ui_append_col(linha, sizeof(linha), &pos, nomeClip, REC_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, alunosStr, REC_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, recMensal, REC_COL_MENSAL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, recAnual, REC_COL_ANUAL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctStr, REC_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_atividades_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Atividade", ATIV_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Planos", ATIV_COL_PLANOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", ATIV_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "%", ATIV_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_atividades_row(const char *nome, int planos, int alunos, double pct)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[48];
    ui_clip_utf8(nome != NULL ? nome : "N/A", 24, nomeClip, sizeof(nomeClip));
    char planosStr[12];
    snprintf(planosStr, sizeof(planosStr), "%d", planos);
    char alunosStr[16];
    snprintf(alunosStr, sizeof(alunosStr), "%d", alunos);
    char pctStr[16];
    snprintf(pctStr, sizeof(pctStr), "%.2f", pct);

    ui_append_col(linha, sizeof(linha), &pos, nomeClip, ATIV_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, planosStr, ATIV_COL_PLANOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, alunosStr, ATIV_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctStr, ATIV_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void moduloRelatoriosPlano(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios de Planos");
        ui_empty_line();
        ui_menu_option('1', "Listagem completa de planos");
        ui_menu_option('2', "Ranking de popularidade");
        ui_menu_option('3', "Analise de receita por plano");
        ui_menu_option('4', "Distribuicao de atividades");
        ui_menu_option('5', "Ocupacao por horario de plano");
        ui_menu_option('6', "Top 5 planos mais lucrativos");
        ui_empty_line();
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        ui_text_line("Use as teclas indicadas para navegar.");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

        op = lerTecla();
        limparTela();

        switch (op)
        {
        case '1':
            relatorioListagemPlanos();
            break;

        case '2':
            relatorioRankingPopularidade();
            break;

        case '3':
            relatorioReceitaPorPlano();
            break;

        case '4':
            relatorioDistribuicaoAtividades();
            break;

        case '5':
            relatorioOcupacaoHorarioPlano();
            break;
        case '6':
            relatorioPlanosMaisLucrativos();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

static void relatorioListagemPlanos(void)
{
    cabecalho_relatorio("Relatorio - Listagem completa de planos");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    char filtroStatus = selecionarStatusPlano();
    char ordenacao = selecionarOrdenacaoPlanos();
    char termo[256];
    solicitarTermoPlanos(termo, sizeof(termo));

    struct PlanoView views[MAX_PLANOS];
    int totalFiltrados = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!planoCorrespondeFiltros(&lista_planos[i], filtroStatus, termo))
        {
            continue;
        }

        int alunos = contarAlunosNoPlano(&lista_planos[i]);
        double receita = alunos * lista_planos[i].valor;

        views[totalFiltrados].plano = &lista_planos[i];
        views[totalFiltrados].alunosVinculados = alunos;
        views[totalFiltrados].receita = receita;
        totalFiltrados++;
    }

    if (totalFiltrados == 0)
    {
        ui_section_title("Nenhum plano encontrado");
        ui_text_line("Ajuste os filtros e tente novamente.");
        aguardar_voltar();
        return;
    }

    ordenarPlanos(views, totalFiltrados, ordenacao);

    cabecalho_relatorio("Relatorio - Listagem completa de planos");

    ui_section_title("Filtros aplicados");
    char linha[UI_INNER + 1];
    const char *statusDesc = filtroStatus == '1' ? "Somente ativos" : (filtroStatus == '2' ? "Somente inativos" : "Todos");
    snprintf(linha, sizeof(linha), "Status: %s | Ordenacao: %s",
             statusDesc,
             ordenacao == '2' ? "Alunos" : (ordenacao == '3' ? "Valor" : "Nome"));
    ui_text_line(linha);
    char termoCurto[32];
    const char *termoOrigem = termo[0] != '\0' ? termo : "Nenhum";
    ui_clip_utf8(termoOrigem, 28, termoCurto, sizeof(termoCurto));
    snprintf(linha, sizeof(linha), "Termo: %s", termoCurto);
    ui_text_line(linha);
    ui_line('-');

    tabela_planos_header();
    double receitaTotal = 0.0;
    int alunosTotais = 0;
    for (int i = 0; i < totalFiltrados; i++)
    {
        tabela_planos_row(&views[i]);
        alunosTotais += views[i].alunosVinculados;
        receitaTotal += views[i].receita;
    }
    ui_line('-');

    ui_section_title("Totais");
    snprintf(linha, sizeof(linha), "Planos listados: %d | Alunos vinculados: %d", totalFiltrados, alunosTotais);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Receita total estimada: R$ %.2f", receitaTotal);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

static void relatorioRankingPopularidade(void)
{
    cabecalho_relatorio("Relatorio - Ranking de popularidade");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct PlanoView ranking[MAX_PLANOS];
    int totalRanking = 0;
    int totalAlunos = 0;

    for (int i = 0; i < total_planos; i++)
    {
        int alunosPlano = contarAlunosNoPlano(&lista_planos[i]);
        ranking[totalRanking].plano = &lista_planos[i];
        ranking[totalRanking].alunosVinculados = alunosPlano;
        ranking[totalRanking].receita = 0.0;
        totalRanking++;
        totalAlunos += alunosPlano;
    }

    if (totalAlunos == 0)
    {
        ui_center_text("Nenhum aluno vinculado a planos para gerar o ranking.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalRanking - 1; i++)
    {
        for (int j = i + 1; j < totalRanking; j++)
        {
            if (ranking[j].alunosVinculados > ranking[i].alunosVinculados)
            {
                struct PlanoView tmp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = tmp;
            }
        }
    }

    tabela_rank_header();
    for (int i = 0; i < totalRanking; i++)
    {
        double percentual = totalAlunos > 0 ? ((double)ranking[i].alunosVinculados / totalAlunos) * 100.0 : 0.0;
        tabela_rank_row(i + 1, &ranking[i], percentual);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de alunos vinculados: %d", totalAlunos);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

static void relatorioReceitaPorPlano(void)
{
    cabecalho_relatorio("Relatorio - Receita por plano");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    char filtroStatus = selecionarStatusPlano();

    struct PlanoView views[MAX_PLANOS];
    int totalViews = 0;
    double receitaTotal = 0.0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!statusPlanoCorresponde(lista_planos[i].ativo, filtroStatus))
        {
            continue;
        }

        int alunos = contarAlunosNoPlano(&lista_planos[i]);
        double receitaMensal = alunos * lista_planos[i].valor;

        views[totalViews].plano = &lista_planos[i];
        views[totalViews].alunosVinculados = alunos;
        views[totalViews].receita = receitaMensal;
        totalViews++;

        receitaTotal += receitaMensal;
    }

    if (totalViews == 0)
    {
        ui_section_title("Nenhum plano atende ao filtro");
        ui_text_line("Ajuste o status e tente novamente.");
        aguardar_voltar();
        return;
    }

    const struct PlanoView *maior = &views[0];
    const struct PlanoView *menor = &views[0];

    for (int i = 0; i < totalViews; i++)
    {
        if (views[i].receita > maior->receita)
        {
            maior = &views[i];
        }
        if (views[i].receita < menor->receita)
        {
            menor = &views[i];
        }
    }

    tabela_receita_header();
    for (int i = 0; i < totalViews; i++)
    {
        double percentual = receitaTotal > 0.0 ? (views[i].receita / receitaTotal) * 100.0 : 0.0;
        tabela_receita_row(&views[i], percentual);
    }
    ui_line('-');

    char linha[UI_INNER + 1];
    ui_section_title("Totais e destaques");
    snprintf(linha, sizeof(linha), "Receita mensal total: R$ %.2f | Receita anual: R$ %.2f", receitaTotal, receitaTotal * 12.0);
    ui_text_line(linha);
    char nomeMaior[48];
    ui_clip_utf8(maior->plano->nome, 30, nomeMaior, sizeof(nomeMaior));
    snprintf(linha, sizeof(linha), "Maior receita: %s (R$ %.2f)", nomeMaior, maior->receita);
    ui_text_line(linha);
    char nomeMenor[48];
    ui_clip_utf8(menor->plano->nome, 30, nomeMenor, sizeof(nomeMenor));
    snprintf(linha, sizeof(linha), "Menor receita: %s (R$ %.2f)", nomeMenor, menor->receita);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

static void relatorioDistribuicaoAtividades(void)
{
    cabecalho_relatorio("Relatorio - Distribuicao de atividades");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct AtividadeView
    {
        char nome[64];
        int planos;
        int alunos;
    };

    struct AtividadeView atividades[MAX_PLANOS * MAX_ATIVIDADES];
    int totalAtividades = 0;
    char bufferLower[64];

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int alunosPlano = contarAlunosNoPlano(&lista_planos[i]);

        for (int j = 0; j < lista_planos[i].total_atividades; j++)
        {
            const char *atividade = lista_planos[i].atividades[j];
            if (atividade == NULL || atividade[0] == '\0')
            {
                continue;
            }

            size_t len = strlen(atividade);
            if (len >= sizeof(bufferLower))
            {
                len = sizeof(bufferLower) - 1;
            }
            for (size_t k = 0; k < len; k++)
            {
                bufferLower[k] = (char)tolower((unsigned char)atividade[k]);
            }
            bufferLower[len] = '\0';

            int indice = -1;
            for (int a = 0; a < totalAtividades; a++)
            {
                if (strcmp(atividades[a].nome, bufferLower) == 0)
                {
                    indice = a;
                    break;
                }
            }

            if (indice == -1 && totalAtividades < (int)(sizeof(atividades) / sizeof(atividades[0])))
            {
                strncpy(atividades[totalAtividades].nome, bufferLower, sizeof(atividades[totalAtividades].nome));
                atividades[totalAtividades].nome[sizeof(atividades[totalAtividades].nome) - 1] = '\0';
                atividades[totalAtividades].planos = 0;
                atividades[totalAtividades].alunos = 0;
                indice = totalAtividades;
                totalAtividades++;
            }

            if (indice != -1)
            {
                atividades[indice].planos++;
                atividades[indice].alunos += alunosPlano;
            }
        }
    }

    if (totalAtividades == 0)
    {
        ui_center_text("Nenhuma atividade cadastrada nos planos.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalAtividades - 1; i++)
    {
        for (int j = i + 1; j < totalAtividades; j++)
        {
            if (atividades[j].alunos > atividades[i].alunos)
            {
                struct AtividadeView tmp = atividades[i];
                atividades[i] = atividades[j];
                atividades[j] = tmp;
            }
        }
    }

    int totalAlunosAcessando = 0;
    for (int i = 0; i < totalAtividades; i++)
    {
        totalAlunosAcessando += atividades[i].alunos;
    }

    tabela_atividades_header();
    for (int i = 0; i < totalAtividades; i++)
    {
        double percentual = totalAlunosAcessando > 0 ? ((double)atividades[i].alunos / totalAlunosAcessando) * 100.0 : 0.0;
        tabela_atividades_row(atividades[i].nome, atividades[i].planos, atividades[i].alunos, percentual);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de atividades analisadas: %d", totalAtividades);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Total de alunos com acesso: %d", totalAlunosAcessando);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

static void relatorioOcupacaoHorarioPlano(void)
{
    cabecalho_relatorio("Relatorio - Ocupacao por horario de plano");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    enum
    {
        FAIXA_MANHA = 0,
        FAIXA_TARDE,
        FAIXA_NOITE,
        FAIXA_OUTROS,
        TOTAL_FAIXAS
    };

    const char *faixasNome[TOTAL_FAIXAS] = {
        "MANHA (05:00 - 12:00)",
        "TARDE (12:00 - 18:00)",
        "NOITE (18:00 - 23:59)",
        "OUTROS"};

    struct PlanoFaixa
    {
        const struct plano *plano;
        int alunos;
    };

    struct PlanoFaixa distribuicao[TOTAL_FAIXAS][MAX_PLANOS];
    int contadores[TOTAL_FAIXAS] = {0};
    int alunosPorFaixa[TOTAL_FAIXAS] = {0};

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int inicio = 0;
        sscanf(lista_planos[i].horario_inicio, "%2d", &inicio);

        int faixa = FAIXA_OUTROS;
        if (inicio >= 5 && inicio < 12)
        {
            faixa = FAIXA_MANHA;
        }
        else if (inicio >= 12 && inicio < 18)
        {
            faixa = FAIXA_TARDE;
        }
        else if (inicio >= 18 && inicio < 24)
        {
            faixa = FAIXA_NOITE;
        }

        int alunos = contarAlunosNoPlano(&lista_planos[i]);

        if (contadores[faixa] < MAX_PLANOS)
        {
            distribuicao[faixa][contadores[faixa]].plano = &lista_planos[i];
            distribuicao[faixa][contadores[faixa]].alunos = alunos;
            contadores[faixa]++;
        }

        alunosPorFaixa[faixa] += alunos;
    }

    for (int faixa = 0; faixa < TOTAL_FAIXAS; faixa++)
    {
        ui_section_title(faixasNome[faixa]);

        if (contadores[faixa] == 0)
        {
            ui_text_line("Nenhum plano nesta faixa.");
            continue;
        }

        for (int i = 0; i < contadores[faixa]; i++)
        {
            char linha[UI_INNER + 1];
            char nomeClip[40];
            ui_clip_utf8(distribuicao[faixa][i].plano->nome, 28, nomeClip, sizeof(nomeClip));
            snprintf(linha, sizeof(linha), "- %s: %d aluno%s", nomeClip,
                     distribuicao[faixa][i].alunos,
                     distribuicao[faixa][i].alunos == 1 ? "" : "s");
            ui_text_line(linha);
        }

        char resumo[UI_INNER + 1];
        snprintf(resumo, sizeof(resumo), "Total na faixa: %d aluno%s",
                 alunosPorFaixa[faixa], alunosPorFaixa[faixa] == 1 ? "" : "s");
        ui_text_line(resumo);
        ui_line('-');
    }

    aguardar_voltar();
}

static void relatorioPlanosMaisLucrativos(void)
{
    cabecalho_relatorio("Relatorio - Top 5 planos mais lucrativos");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct PlanoView views[MAX_PLANOS];
    int total = 0;
    double receitaTotal = 0.0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }
        int alunos = contarAlunosNoPlano(&lista_planos[i]);
        double receita = alunos * lista_planos[i].valor;
        views[total].plano = &lista_planos[i];
        views[total].alunosVinculados = alunos;
        views[total].receita = receita;
        receitaTotal += receita;
        total++;
    }

    if (total == 0)
    {
        ui_center_text("Nao ha planos ativos para calcular receita.");
        aguardar_voltar();
        return;
    }

    quicksortPlanosPorReceita(views, 0, total - 1);

    int limite = total < 5 ? total : 5;

    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Rank", TOP_COL_RANK);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", TOP_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", TOP_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Rec. M", TOP_COL_REC_M);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Rec. A", TOP_COL_REC_A);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "%", TOP_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');

    for (int i = 0; i < limite; i++)
    {
        double percentual = receitaTotal > 0.0 ? (views[i].receita / receitaTotal) * 100.0 : 0.0;
        pos = 0;
        char rankStr[8];
        snprintf(rankStr, sizeof(rankStr), "%d", i + 1);
        char nomeClip[32];
        ui_clip_utf8(views[i].plano->nome, 16, nomeClip, sizeof(nomeClip));
        char alunosStr[16];
        snprintf(alunosStr, sizeof(alunosStr), "%d", views[i].alunosVinculados);
        char recMensal[24];
        snprintf(recMensal, sizeof(recMensal), "%.2f", views[i].receita);
        char recAnual[24];
        snprintf(recAnual, sizeof(recAnual), "%.2f", views[i].receita * 12.0);
        char pctStr[16];
        snprintf(pctStr, sizeof(pctStr), "%.2f", percentual);

        ui_append_col(linha, sizeof(linha), &pos, rankStr, TOP_COL_RANK);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, nomeClip, TOP_COL_NOME);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, alunosStr, TOP_COL_ALUNOS);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, recMensal, TOP_COL_REC_M);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, recAnual, TOP_COL_REC_A);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, pctStr, TOP_COL_PCT);
        linha[pos] = '\0';
        ui_text_line(linha);
    }

    ui_line('-');
    snprintf(linha, sizeof(linha), "Planos considerados: %d | Receita total mensal: %.2f", total, receitaTotal);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

static char selecionarStatusPlano(void)
{
    while (1)
    {
        cabecalho_relatorio("Filtro - Status dos planos");
        ui_menu_option('1', "Somente ativos");
        ui_menu_option('2', "Somente inativos");
        ui_menu_option('3', "Todos");
        ui_section_title("Escolha uma opcao");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

        char op = lerTecla();
        if (op == '1' || op == '2' || op == '3')
        {
            return op;
        }

        opInvalida();
    }
}

static void solicitarTermoPlanos(char *dest, size_t tamanho)
{
    cabecalho_relatorio("Filtro - Termo de busca");
    ui_text_line("Digite parte do nome, ID ou atividade para buscar (ENTER para ignorar):");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
    if (fgets(dest, tamanho, stdin) != NULL)
    {
        dest[strcspn(dest, "\n")] = '\0';
    }
    else if (tamanho > 0)
    {
        dest[0] = '\0';
    }
}

static char selecionarOrdenacaoPlanos(void)
{
    while (1)
    {
        cabecalho_relatorio("Filtro - Ordenacao de planos");
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Quantidade de alunos");
        ui_menu_option('3', "Valor do plano");
        ui_section_title("Escolha uma opcao");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

        char op = lerTecla();
        if (op == '1' || op == '2' || op == '3')
        {
            return op;
        }

        opInvalida();
    }
}

static bool planoCorrespondeFiltros(const struct plano *plano, char statusFiltro, const char *termo)
{
    if (!statusPlanoCorresponde(plano->ativo, statusFiltro))
    {
        return false;
    }

    if (termo[0] == '\0')
    {
        return true;
    }

    if (textoContemPlano(plano->nome, termo) || textoContemPlano(plano->id, termo))
    {
        return true;
    }

    for (int i = 0; i < plano->total_atividades; i++)
    {
        if (textoContemPlano(plano->atividades[i], termo))
        {
            return true;
        }
    }

    return false;
}

static bool statusPlanoCorresponde(bool ativo, char statusFiltro)
{
    switch (statusFiltro)
    {
    case '1':
        return ativo;
    case '2':
        return !ativo;
    case '3':
    default:
        return true;
    }
}

static bool textoContemPlano(const char *texto, const char *busca)
{
    if (texto == NULL || busca == NULL)
    {
        return false;
    }

    if (busca[0] == '\0')
    {
        return true;
    }

    size_t lenTexto = strlen(texto);
    size_t lenBusca = strlen(busca);

    for (size_t i = 0; i + lenBusca <= lenTexto; i++)
    {
        size_t j = 0;
        while (j < lenBusca &&
               tolower((unsigned char)texto[i + j]) == tolower((unsigned char)busca[j]))
        {
            j++;
        }
        if (j == lenBusca)
        {
            return true;
        }
    }

    return false;
}

static void ordenarPlanos(struct PlanoView *lista, int total, char criterio)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = i + 1; j < total; j++)
        {
            if (compararPlanosView(&lista[i], &lista[j], criterio) > 0)
            {
                struct PlanoView tmp = lista[i];
                lista[i] = lista[j];
                lista[j] = tmp;
            }
        }
    }
}

static int compararPlanosView(const struct PlanoView *a, const struct PlanoView *b, char criterio)
{
    switch (criterio)
    {
    case '2':
        if (b->alunosVinculados != a->alunosVinculados)
        {
            return b->alunosVinculados - a->alunosVinculados;
        }
        break;
    case '3':
        if (b->plano->valor > a->plano->valor)
        {
            return 1;
        }
        if (b->plano->valor < a->plano->valor)
        {
            return -1;
        }
        break;
    case '1':
    default:
        break;
    }

    int comp = compararStringsInsensitive(a->plano->nome, b->plano->nome);
    if (comp != 0)
    {
        return comp;
    }
    return compararStringsInsensitive(a->plano->id, b->plano->id);
}

/*
 * Quicksort customizado para ordenar PlanoView por receita (decrescente).
 * - Escolhe o pivô no meio do segmento atual.
 * - Particiona movendo menores receitas para a direita e maiores para a esquerda.
 * - Recursa em cada sublista ate tamanho 1.
 */
static void quicksortPlanosPorReceita(struct PlanoView *lista, int inicio, int fim)
{
    if (inicio >= fim)
    {
        return; // caso base: sublista de tamanho 0 ou 1
    }

    int i = inicio;
    int j = fim;
    double pivot = lista[(inicio + fim) / 2].receita; // pivô no meio

    while (i <= j)
    {
        while (lista[i].receita > pivot)
        {
            i++; // avança até encontrar elemento menor que pivô
        }
        while (lista[j].receita < pivot)
        {
            j--; // recua até encontrar elemento maior que pivô
        }
        if (i <= j)
        {
            struct PlanoView tmp = lista[i];
            lista[i] = lista[j];
            lista[j] = tmp;
            i++;
            j--;
        }
    }

    // Recursão em cada partição
    if (inicio < j)
    {
        quicksortPlanosPorReceita(lista, inicio, j);
    }
    if (i < fim)
    {
        quicksortPlanosPorReceita(lista, i, fim);
    }
}

static int compararStringsInsensitive(const char *a, const char *b)
{
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        int ca = tolower((unsigned char)a[i]);
        int cb = tolower((unsigned char)b[i]);
        if (ca != cb)
        {
            return ca - cb;
        }
        i++;
    }
    return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]);
}

static int contarAlunosNoPlano(const struct plano *plano)
{
    if (plano == NULL)
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
        if (strcmp(lista_alunos[i].plano_id, plano->id) == 0)
        {
            total++;
        }
    }
    return total;
}
