#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosEquipamento.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Relatorios do modulo de equipamentos: listagem, categorias, cronogramas e filtros
   de manutencao. Usa os vetores globais de equipamentos carregados pelo app. */

#define LIST_COL_ID 8
#define LIST_COL_NOME 18
#define LIST_COL_CATEG 14
#define LIST_COL_PROX 12
#define LIST_COL_STATUS 12

#define CAT_COL_NOME 30
#define CAT_COL_QTD 10
#define CAT_COL_PCT 10

#define CRONO_COL_ID 8
#define CRONO_COL_NOME 18
#define CRONO_COL_DATA 12
#define CRONO_COL_STATUS 14

struct EquipamentoView
{
    const struct equipamento *equip;
    double diasRestantes;
};

static char filtroCategoriaSelecionado;
static char filtroManutencaoSelecionado;
static char criterioOrdenacaoSelecionado;

static void relatorioListagemEquipamentos(void);
static void relatorioEquipamentosPorCategoria(void);
static void relatorioCronogramaManutencoes(void);
static void relatorioManutencoesVencidas(void);
static void relatorioManutencoesProximas(void);
static void relatorioHistoricoManutencoes(void);
static void relatorioEquipamentosAgrupadosPorCategoria(void);
static char selecionarFiltroCategoria(void);
static char selecionarFiltroManutencao(void);
static char selecionarOrdenacaoEquipamentos(void);
static int compararEquipamentos(const void *a, const void *b);
static double calcularDiasParaManutencao(const char *data);
static void cabecalho_relatorio(const char *subtitulo);
static void aguardar_voltar(void);
static void tabela_listagem_header(void);
static void tabela_listagem_row(const struct EquipamentoView *view, const char *status);
static void tabela_categoria_resumo_header(void);
static void tabela_categoria_resumo_row(const char *nome, int quantidade, double pct);
static void tabela_crono_header(const char *titulo);
static void tabela_crono_row(const struct equipamento *eq, const char *status);

static void cabecalho_relatorio(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Pausa padrao para leitura antes de voltar ao menu. */
static void aguardar_voltar(void)
{
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

/* Cabecalho/linha da listagem principal (ID, nome, categoria, proxima manutencao). */
static void tabela_listagem_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", LIST_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", LIST_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Categoria", LIST_COL_CATEG);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Prox. Manut", LIST_COL_PROX);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", LIST_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_listagem_row(const struct EquipamentoView *view, const char *status)
{
    if (view == NULL || view->equip == NULL)
    {
        return;
    }

    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, view->equip->id, LIST_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, view->equip->nome, LIST_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, view->equip->categoria, LIST_COL_CATEG);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, view->equip->proxima_manutencao, LIST_COL_PROX);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status, LIST_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_categoria_resumo_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Categoria", CAT_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Qtd", CAT_COL_QTD);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "%", CAT_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_categoria_resumo_row(const char *nome, int quantidade, double pct)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[48];
    ui_clip_utf8(nome != NULL ? nome : "N/A", 30, nomeClip, sizeof(nomeClip));
    char qtdStr[12];
    snprintf(qtdStr, sizeof(qtdStr), "%d", quantidade);
    char pctStr[16];
    snprintf(pctStr, sizeof(pctStr), "%.2f", pct);

    ui_append_col(linha, sizeof(linha), &pos, nomeClip, CAT_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, qtdStr, CAT_COL_QTD);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, pctStr, CAT_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_crono_header(const char *titulo)
{
    ui_section_title(titulo);
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", CRONO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", CRONO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Data", CRONO_COL_DATA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", CRONO_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_crono_row(const struct equipamento *eq, const char *status)
{
    if (eq == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, eq->id, CRONO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->nome, CRONO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, eq->proxima_manutencao, CRONO_COL_DATA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status, CRONO_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void moduloRelatoriosEquipamento(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios de Equipamentos");
        ui_empty_line();
        ui_menu_option('1', "Listagem completa de equipamentos");
        ui_menu_option('2', "Equipamentos por categoria");
        ui_menu_option('3', "Cronograma de manutencoes");
        ui_menu_option('4', "Manutencoes vencidas (urgente)");
        ui_menu_option('5', "Manutencoes proximas (7/15/30 dias)");
        ui_menu_option('6', "Historico de manutencoes");
        ui_menu_option('7', "Agrupados por categoria (sumario)");
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
            relatorioListagemEquipamentos();
            break;
        case '2':
            relatorioEquipamentosPorCategoria();
            break;
        case '3':
            relatorioCronogramaManutencoes();
            break;
        case '4':
            relatorioManutencoesVencidas();
            break;
        case '5':
            relatorioManutencoesProximas();
            break;
        case '6':
            relatorioHistoricoManutencoes();
            break;
        case '7':
            relatorioEquipamentosAgrupadosPorCategoria();
            break;
        case '0':
            break;
        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

static void relatorioListagemEquipamentos(void)
{
    cabecalho_relatorio("Relatorio - Listagem completa de equipamentos");

    if (total_equipamentos == 0)
    {
        ui_center_text("Nao ha equipamentos cadastrados.");
        aguardar_voltar();
        return;
    }

    filtroCategoriaSelecionado = selecionarFiltroCategoria();
    filtroManutencaoSelecionado = selecionarFiltroManutencao();
    criterioOrdenacaoSelecionado = selecionarOrdenacaoEquipamentos();

    struct EquipamentoView lista[MAX_EQUIPAMENTOS];
    int totalLista = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (filtroCategoriaSelecionado != '0' &&
            tolower((unsigned char)filtroCategoriaSelecionado) != tolower((unsigned char)lista_equipamentos[i].categoria[0]))
        {
            continue;
        }

        double dias = calcularDiasParaManutencao(lista_equipamentos[i].proxima_manutencao);
        bool atendeStatus = true;

        if (filtroManutencaoSelecionado == '1' && dias >= 0)
        {
            atendeStatus = false;
        }
        else if (filtroManutencaoSelecionado == '2' && (dias < 0 || dias > 7))
        {
            atendeStatus = false;
        }
        else if (filtroManutencaoSelecionado == '3' && (dias < 0 || dias > 15))
        {
            atendeStatus = false;
        }
        else if (filtroManutencaoSelecionado == '4' && (dias < 0 || dias > 30))
        {
            atendeStatus = false;
        }

        if (!atendeStatus)
        {
            continue;
        }

        lista[totalLista].equip = &lista_equipamentos[i];
        lista[totalLista].diasRestantes = dias;
        totalLista++;
    }

    if (totalLista == 0)
    {
        ui_section_title("Nenhum equipamento encontrado");
        ui_text_line("Ajuste os filtros e tente novamente.");
        aguardar_voltar();
        return;
    }

    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    cabecalho_relatorio("Relatorio - Listagem completa de equipamentos");
    ui_section_title("Filtros aplicados");
    char linha[UI_INNER + 1];
    char catFiltro[24];
    if (filtroCategoriaSelecionado == '0')
    {
        strncpy(catFiltro, "Todas", sizeof(catFiltro));
    }
    else
    {
        snprintf(catFiltro, sizeof(catFiltro), "Inicial '%c'", filtroCategoriaSelecionado);
    }
    const char *statusFiltro = "Todos";
    switch (filtroManutencaoSelecionado)
    {
    case '1':
        statusFiltro = "Somente vencidas";
        break;
    case '2':
        statusFiltro = "Proximas 7 dias";
        break;
    case '3':
        statusFiltro = "Proximas 15 dias";
        break;
    case '4':
        statusFiltro = "Proximas 30 dias";
        break;
    default:
        statusFiltro = "Todos";
        break;
    }
    const char *ordenacao = criterioOrdenacaoSelecionado == '2' ? "Categoria" : (criterioOrdenacaoSelecionado == '3' ? "Proxima manutencao" : "Nome");
    snprintf(linha, sizeof(linha), "Categoria: %s | Manutencao: %s", catFiltro, statusFiltro);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Ordenacao: %s", ordenacao);
    ui_text_line(linha);
    ui_line('-');

    tabela_listagem_header();
    for (int i = 0; i < totalLista; i++)
    {
        const struct EquipamentoView *vw = &lista[i];
        const char *status = "Em dia";
        if (vw->diasRestantes < 0)
        {
            status = "Vencida";
        }
        else if (vw->diasRestantes <= 7)
        {
            status = "Proxima";
        }
        tabela_listagem_row(vw, status);
    }
    ui_line('-');
    snprintf(linha, sizeof(linha), "Total listado: %d", totalLista);
    ui_text_line(linha);
    ui_text_line("Sugestao: revisar manutencoes pendentes e atualizar o cronograma.");
    ui_line('-');

    aguardar_voltar();
}

/* Distribui equipamentos por categoria (case-insensitive) e lista cada grupo. */
static void relatorioEquipamentosPorCategoria(void)
{
    cabecalho_relatorio("Relatorio - Equipamentos por categoria");

    if (total_equipamentos == 0)
    {
        ui_center_text("Nao ha equipamentos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct CategoriaView
    {
        char nome[128];
        const struct equipamento *itens[MAX_EQUIPAMENTOS];
        int total;
    };

    struct CategoriaView categorias[MAX_EQUIPAMENTOS];
    int totalCategorias = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].nome[0] == '\0')
        {
            continue;
        }

        char categoriaUpper[128];
        size_t len = strlen(lista_equipamentos[i].categoria);
        if (len >= sizeof(categoriaUpper))
        {
            len = sizeof(categoriaUpper) - 1;
        }

        for (size_t c = 0; c < len; c++)
        {
            categoriaUpper[c] = (char)toupper((unsigned char)lista_equipamentos[i].categoria[c]);
        }
        categoriaUpper[len] = '\0';

        int indice = -1;
        for (int cat = 0; cat < totalCategorias; cat++)
        {
            if (strcmp(categorias[cat].nome, categoriaUpper) == 0)
            {
                indice = cat;
                break;
            }
        }

        if (indice == -1 && totalCategorias < MAX_EQUIPAMENTOS)
        {
            strncpy(categorias[totalCategorias].nome, categoriaUpper, sizeof(categorias[totalCategorias].nome));
            categorias[totalCategorias].nome[sizeof(categorias[totalCategorias].nome) - 1] = '\0';
            categorias[totalCategorias].total = 0;
            indice = totalCategorias;
            totalCategorias++;
        }

        if (indice != -1 && categorias[indice].total < MAX_EQUIPAMENTOS)
        {
            categorias[indice].itens[categorias[indice].total++] = &lista_equipamentos[i];
        }
    }

    if (totalCategorias == 0)
    {
        ui_section_title("Nao foi possivel agrupar");
        ui_text_line("Nenhum equipamento encontrado.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalCategorias - 1; i++)
    {
        for (int j = i + 1; j < totalCategorias; j++)
        {
            if (strcmp(categorias[j].nome, categorias[i].nome) < 0)
            {
                struct CategoriaView tmp = categorias[i];
                categorias[i] = categorias[j];
                categorias[j] = tmp;
            }
        }
    }

    for (int i = 0; i < totalCategorias; i++)
    {
        char titulo[UI_INNER + 1];
        char nomeClip[48];
        ui_clip_utf8(categorias[i].nome, 30, nomeClip, sizeof(nomeClip));
        snprintf(titulo, sizeof(titulo), "%s (%d equipamento%s)",
                 nomeClip,
                 categorias[i].total,
                 categorias[i].total == 1 ? "" : "s");
        ui_section_title(titulo);

        if (categorias[i].total == 0)
        {
            ui_text_line("Nenhum equipamento nesta categoria.");
            continue;
        }

        for (int j = 0; j < categorias[i].total; j++)
        {
            const struct equipamento *eq = categorias[i].itens[j];
            char linha[160];
            char nomeEq[48];
            ui_clip_utf8(eq->nome, 38, nomeEq, sizeof(nomeEq));
            snprintf(linha, sizeof(linha), "- [%s] %s %s",
                     eq->id,
                     nomeEq,
                     eq->ativo ? "" : "(Inativo)");
            ui_text_line(linha);
        }
    }

    ui_line('-');
    aguardar_voltar();
}

static void relatorioEquipamentosAgrupadosPorCategoria(void)
{
    cabecalho_relatorio("Relatorio - Equipamentos agrupados por categoria");

    if (total_equipamentos == 0)
    {
        ui_center_text("Nao ha equipamentos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct GrupoCategoria
    {
        char categoria[64];
        int quantidade;
        double percentual;
    };

    struct GrupoCategoria grupos[MAX_EQUIPAMENTOS];
    int totalGrupos = 0;
    int totalConsiderados = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].nome[0] == '\0')
        {
            continue;
        }

        char cat[64];
        strncpy(cat, lista_equipamentos[i].categoria, sizeof(cat));
        cat[sizeof(cat) - 1] = '\0';

        int idx = -1;
        for (int g = 0; g < totalGrupos; g++)
        {
            if (strcmp(grupos[g].categoria, cat) == 0)
            {
                idx = g;
                break;
            }
        }
        if (idx == -1 && totalGrupos < MAX_EQUIPAMENTOS)
        {
            strncpy(grupos[totalGrupos].categoria, cat, sizeof(grupos[totalGrupos].categoria));
            grupos[totalGrupos].categoria[sizeof(grupos[totalGrupos].categoria) - 1] = '\0';
            grupos[totalGrupos].quantidade = 0;
            grupos[totalGrupos].percentual = 0.0;
            idx = totalGrupos;
            totalGrupos++;
        }

        if (idx != -1)
        {
            grupos[idx].quantidade++;
            totalConsiderados++;
        }
    }

    if (totalGrupos == 0 || totalConsiderados == 0)
    {
        ui_section_title("Nao ha dados suficientes");
        ui_text_line("Nao foi possivel agrupar equipamentos por categoria.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalGrupos; i++)
    {
        grupos[i].percentual = (double)grupos[i].quantidade / totalConsiderados * 100.0;
    }

    for (int i = 0; i < totalGrupos - 1; i++)
    {
        for (int j = i + 1; j < totalGrupos; j++)
        {
            if (grupos[j].quantidade > grupos[i].quantidade ||
                (grupos[j].quantidade == grupos[i].quantidade &&
                 strcmp(grupos[j].categoria, grupos[i].categoria) < 0))
            {
                struct GrupoCategoria tmp = grupos[i];
                grupos[i] = grupos[j];
                grupos[j] = tmp;
            }
        }
    }

    tabela_categoria_resumo_header();
    for (int i = 0; i < totalGrupos; i++)
    {
        tabela_categoria_resumo_row(grupos[i].categoria, grupos[i].quantidade, grupos[i].percentual);
    }
    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de equipamentos considerados: %d", totalConsiderados);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

static void relatorioCronogramaManutencoes(void)
{
    cabecalho_relatorio("Relatorio - Cronograma de manutencoes");

    if (total_equipamentos == 0)
    {
        ui_center_text("Nao ha equipamentos cadastrados.");
        aguardar_voltar();
        return;
    }

    struct EquipamentoView lista[MAX_EQUIPAMENTOS];
    int totalLista = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].proxima_manutencao[0] == '\0')
        {
            continue;
        }
        lista[totalLista].equip = &lista_equipamentos[i];
        lista[totalLista].diasRestantes = calcularDiasParaManutencao(lista_equipamentos[i].proxima_manutencao);
        totalLista++;
    }

    if (totalLista == 0)
    {
        ui_center_text("Nao ha manutencoes agendadas.");
        aguardar_voltar();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    tabela_crono_header("Vencidas");
    bool temVencidas = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes < 0)
        {
            tabela_crono_row(lista[i].equip, "Vencida");
            temVencidas = true;
        }
    }
    if (!temVencidas)
    {
        ui_text_line("Nenhuma manutencao vencida.");
    }

    tabela_crono_header("Proximos 7 dias");
    bool temSemana = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes >= 0 && lista[i].diasRestantes <= 7)
        {
            tabela_crono_row(lista[i].equip, "Ate 7 dias");
            temSemana = true;
        }
    }
    if (!temSemana)
    {
        ui_text_line("Nenhuma manutencao nesta janela.");
    }

    tabela_crono_header("8 a 15 dias");
    bool temQuinze = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes > 7 && lista[i].diasRestantes <= 15)
        {
            tabela_crono_row(lista[i].equip, "8-15 dias");
            temQuinze = true;
        }
    }
    if (!temQuinze)
    {
        ui_text_line("Nenhuma manutencao entre 8 e 15 dias.");
    }

    tabela_crono_header("16 a 90 dias");
    bool temNoventa = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes > 15 && lista[i].diasRestantes <= 90)
        {
            tabela_crono_row(lista[i].equip, "16-90 dias");
            temNoventa = true;
        }
    }
    if (!temNoventa)
    {
        ui_text_line("Nenhuma manutencao ate 90 dias.");
    }

    ui_line('-');
    aguardar_voltar();
}

static void relatorioManutencoesVencidas(void)
{
    cabecalho_relatorio("Relatorio - Manutencoes vencidas");

    struct EquipamentoView lista[MAX_EQUIPAMENTOS];
    int totalLista = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        double dias = calcularDiasParaManutencao(lista_equipamentos[i].proxima_manutencao);
        if (dias < 0)
        {
            lista[totalLista].equip = &lista_equipamentos[i];
            lista[totalLista].diasRestantes = dias;
            totalLista++;
        }
    }

    if (totalLista == 0)
    {
        ui_center_text("Nao ha manutencoes vencidas no momento.");
        aguardar_voltar();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    tabela_crono_header("Urgente - vencidas");
    for (int i = 0; i < totalLista; i++)
    {
        char status[32];
        snprintf(status, sizeof(status), "Ha %.0f d", -lista[i].diasRestantes);
        tabela_crono_row(lista[i].equip, status);
    }
    ui_line('-');
    aguardar_voltar();
}

static void relatorioManutencoesProximas(void)
{
    cabecalho_relatorio("Relatorio - Manutencoes proximas (7/15/30 dias)");

    ui_menu_option('1', "Proximos 7 dias");
    ui_menu_option('2', "Proximos 15 dias");
    ui_menu_option('3', "Proximos 30 dias");
    ui_section_title("Escolha um periodo");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    char op = lerTecla();
    int limiteDias = 7;
    switch (op)
    {
    case '2':
        limiteDias = 15;
        break;
    case '3':
        limiteDias = 30;
        break;
    default:
        limiteDias = 7;
        break;
    }

    struct EquipamentoView lista[MAX_EQUIPAMENTOS];
    int totalLista = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        double dias = calcularDiasParaManutencao(lista_equipamentos[i].proxima_manutencao);
        if (dias >= 0 && dias <= limiteDias)
        {
            lista[totalLista].equip = &lista_equipamentos[i];
            lista[totalLista].diasRestantes = dias;
            totalLista++;
        }
    }

    if (totalLista == 0)
    {
        char msg[UI_INNER + 1];
        snprintf(msg, sizeof(msg), "Nao ha manutencoes previstas nos proximos %d dias.", limiteDias);
        ui_text_line(msg);
        aguardar_voltar();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    char titulo[UI_INNER + 1];
    snprintf(titulo, sizeof(titulo), "Proximos %d dias", limiteDias);
    tabela_crono_header(titulo);
    for (int i = 0; i < totalLista; i++)
    {
        char status[32];
        snprintf(status, sizeof(status), "Em %.0f d", lista[i].diasRestantes);
        tabela_crono_row(lista[i].equip, status);
    }
    ui_line('-');
    aguardar_voltar();
    aguardar_voltar();
}

static void relatorioHistoricoManutencoes(void)
{
    cabecalho_relatorio("Historico de manutencoes");
    ui_center_text("Funcionalidade em desenvolvimento.");
    aguardar_voltar();
}

/* Filtros e ordenacao usados em varios relatorios. */
static char selecionarFiltroCategoria(void)
{
    cabecalho_relatorio("Filtro - Categoria");
    ui_text_line("Digite a letra inicial da categoria ou 0 para todas.");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
    char c = lerTecla();
    if (c == '\n' || c == '0')
    {
        return '0';
    }
    return c;
}

static char selecionarFiltroManutencao(void)
{
    cabecalho_relatorio("Filtro - Status de manutencao");
    ui_menu_option('0', "Todos");
    ui_menu_option('1', "Somente vencidas");
    ui_menu_option('2', "Proximas 7 dias");
    ui_menu_option('3', "Proximas 15 dias");
    ui_menu_option('4', "Proximas 30 dias");
    ui_section_title("Escolha uma opcao");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    char op = lerTecla();
    if (op < '0' || op > '4')
    {
        opInvalida();
        return selecionarFiltroManutencao();
    }
    return op;
}

static char selecionarOrdenacaoEquipamentos(void)
{
    cabecalho_relatorio("Filtro - Ordenacao de equipamentos");
    ui_menu_option('1', "Nome");
    ui_menu_option('2', "Categoria");
    ui_menu_option('3', "Proxima manutencao");
    ui_section_title("Escolha uma opcao");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);

    char op = lerTecla();
    if (op == '1' || op == '2' || op == '3')
    {
        criterioOrdenacaoSelecionado = op;
        return op;
    }

    opInvalida();
    return selecionarOrdenacaoEquipamentos();
}

static int compararEquipamentosPorNome(const void *a, const void *b)
{
    const struct EquipamentoView *ea = (const struct EquipamentoView *)a;
    const struct EquipamentoView *eb = (const struct EquipamentoView *)b;
    return strcmp(ea->equip->nome, eb->equip->nome);
}

/* Ordenacao secundaria: primeiro categoria, depois nome. */
static int compararEquipamentosPorCategoria(const void *a, const void *b)
{
    const struct EquipamentoView *ea = (const struct EquipamentoView *)a;
    const struct EquipamentoView *eb = (const struct EquipamentoView *)b;
    int comp = strcmp(ea->equip->categoria, eb->equip->categoria);
    if (comp != 0)
    {
        return comp;
    }
    return strcmp(ea->equip->nome, eb->equip->nome);
}

static int compararEquipamentosPorData(const void *a, const void *b)
{
    const struct EquipamentoView *ea = (const struct EquipamentoView *)a;
    const struct EquipamentoView *eb = (const struct EquipamentoView *)b;

    if (ea->diasRestantes < eb->diasRestantes)
    {
        return -1;
    }
    if (ea->diasRestantes > eb->diasRestantes)
    {
        return 1;
    }
    return strcmp(ea->equip->nome, eb->equip->nome);
}

static int compararEquipamentos(const void *a, const void *b)
{
    switch (criterioOrdenacaoSelecionado)
    {
    case '2':
        return compararEquipamentosPorCategoria(a, b);
    case '3':
        return compararEquipamentosPorData(a, b);
    case '1':
    default:
        return compararEquipamentosPorNome(a, b);
    }
}

static double calcularDiasParaManutencao(const char *data)
{
    struct tm tm_alvo = {0};
    if (strptime(data, "%d/%m/%Y", &tm_alvo) == NULL)
    {
        return 0.0;
    }

    time_t t_alvo = mktime(&tm_alvo);
    time_t agora = time(NULL);

    return difftime(t_alvo, agora) / (60 * 60 * 24);
}
