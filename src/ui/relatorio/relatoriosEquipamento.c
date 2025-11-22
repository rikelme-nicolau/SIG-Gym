#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosEquipamento.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"
#include "ui/utils/lerTecla.h"

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

void moduloRelatoriosEquipamento(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===              RELATORIOS DE EQUIPAMENTOS - MENU            ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  LISTAGEM COMPLETA DE EQUIPAMENTOS                   ===\n");
        printf("===  [2]  EQUIPAMENTOS POR CATEGORIA                          ===\n");
        printf("===  [3]  CRONOGRAMA DE MANUTENCOES                           ===\n");
        printf("===  [4]  MANUTENCOES VENCIDAS (URGENTE)                      ===\n");
        printf("===  [5]  MANUTENCOES PROXIMAS (7/15/30 DIAS)                 ===\n");
        printf("===  [6]  HISTORICO DE MANUTENCOES                            ===\n");
        printf("===  [7]  AGRUPADOS POR CATEGORIA (SUMARIO)                   ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

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
    limparTela();

    printf("=========================================================================\n");
    printf("===            RELATORIO - LISTAGEM COMPLETA DE EQUIPAMENTOS         ===\n");
    printf("=========================================================================\n");

    if (total_equipamentos == 0)
    {
        printf("Nao ha equipamentos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("Nenhum equipamento atende aos filtros informados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    printf("+----------+----------------------+----------------------+---------------+-----------+\n");
    printf("| ID       | Nome                 | Categoria            | Prox. Manut.  | Status    |\n");
    printf("+----------+----------------------+----------------------+---------------+-----------+\n");

    for (int i = 0; i < totalLista; i++)
    {
        const struct equipamento *eq = lista[i].equip;
        double dias = lista[i].diasRestantes;
        const char *icone = "✓ Em dia";

        if (dias < 0)
        {
            icone = "✗ Vencida";
        }
        else if (dias <= 7)
        {
            icone = "⚠ Proxima";
        }

        printf("| %-8.8s | %-20.20s | %-20.20s | %-13.13s | %-9.9s |\n",
               eq->id,
               eq->nome,
               eq->categoria,
               eq->proxima_manutencao,
               icone);
    }

    printf("+----------+----------------------+----------------------+---------------+-----------+\n");
    printf("Total listado: %d\n", totalLista);
    printf("Sugestao: revisar status de manutencao e vincular acao conforme necessario.\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioEquipamentosPorCategoria(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===             RELATORIO - EQUIPAMENTOS POR CATEGORIA               ===\n");
    printf("=========================================================================\n");

    if (total_equipamentos == 0)
    {
        printf("Nao ha equipamentos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("Nao foi possivel agrupar equipamentos por categoria.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("\n%s (%d equipamento%s)\n",
               categorias[i].nome,
               categorias[i].total,
               categorias[i].total == 1 ? "" : "s");
        printf("--------------------------------------------------------------\n");

        for (int j = 0; j < categorias[i].total; j++)
        {
            const struct equipamento *eq = categorias[i].itens[j];
            printf("  - [%s] %s %s\n",
                   eq->id,
                   eq->nome,
                   eq->ativo ? "" : "(Inativo)");
        }
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioEquipamentosAgrupadosPorCategoria(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===         RELATORIO - EQUIPAMENTOS AGRUPADOS POR CATEGORIA         ===\n");
    printf("=========================================================================\n");

    if (total_equipamentos == 0)
    {
        printf("Nao ha equipamentos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("Nao foi possivel agrupar equipamentos por categoria.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
            if (grupos[j].quantidade > grupos[i].quantidade)
            {
                struct GrupoCategoria tmp = grupos[i];
                grupos[i] = grupos[j];
                grupos[j] = tmp;
            }
            else if (grupos[j].quantidade == grupos[i].quantidade &&
                     strcmp(grupos[j].categoria, grupos[i].categoria) < 0)
            {
                struct GrupoCategoria tmp = grupos[i];
                grupos[i] = grupos[j];
                grupos[j] = tmp;
            }
        }
    }

    printf("\n+------------------------------+------------+-------------+\n");
    printf("| %-28s | %-10s | %-9s |\n", "Categoria", "Quantidade", "%");
    printf("+------------------------------+------------+-------------+\n");
    for (int i = 0; i < totalGrupos; i++)
    {
        printf("| %-28.28s | %10d | %9.2f |\n",
               grupos[i].categoria,
               grupos[i].quantidade,
               grupos[i].percentual);
    }
    printf("+------------------------------+------------+-------------+\n");
    printf("Total de equipamentos considerados: %d\n", totalConsiderados);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioCronogramaManutencoes(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===              RELATORIO - CRONOGRAMA DE MANUTENCOES               ===\n");
    printf("=========================================================================\n");

    if (total_equipamentos == 0)
    {
        printf("Nao ha equipamentos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("Nao ha manutencoes agendadas.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    printf("PRÓXIMOS 90 DIAS\n");
    printf("------------------------------\n");

    printf("\nVENCIDAS:\n");
    bool temVencidas = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes < 0)
        {
            printf("  [%s] %s - Vencida ha %.0f dia%s\n",
                   lista[i].equip->id,
                   lista[i].equip->nome,
                   -lista[i].diasRestantes,
                   (int)(-lista[i].diasRestantes) == 1 ? "" : "s");
            temVencidas = true;
        }
    }
    if (!temVencidas)
    {
        printf("  Nenhuma manutencao vencida.\n");
    }

    printf("\nESTA SEMANA (proximos 7 dias):\n");
    bool temSemana = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes >= 0 && lista[i].diasRestantes <= 7)
        {
            printf("  [%s] %s - em %.0f dia%s (%s)\n",
                   lista[i].equip->id,
                   lista[i].equip->nome,
                   lista[i].diasRestantes,
                   (int)(lista[i].diasRestantes) == 1 ? "" : "s",
                   lista[i].equip->proxima_manutencao);
            temSemana = true;
        }
    }
    if (!temSemana)
    {
        printf("  Nenhuma manutencao para esta semana.\n");
    }

    printf("\nPROXIMAS 2 SEMANAS (8-15 dias):\n");
    bool temQuinze = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes > 7 && lista[i].diasRestantes <= 15)
        {
            printf("  [%s] %s - em %.0f dias (%s)\n",
                   lista[i].equip->id,
                   lista[i].equip->nome,
                   lista[i].diasRestantes,
                   lista[i].equip->proxima_manutencao);
            temQuinze = true;
        }
    }
    if (!temQuinze)
    {
        printf("  Nenhuma manutencao entre 8 e 15 dias.\n");
    }

    printf("\nPROXIMOS 90 DIAS:\n");
    bool temNoventa = false;
    for (int i = 0; i < totalLista; i++)
    {
        if (lista[i].diasRestantes > 15 && lista[i].diasRestantes <= 90)
        {
            printf("  [%s] %s - em %.0f dias (%s)\n",
                   lista[i].equip->id,
                   lista[i].equip->nome,
                   lista[i].diasRestantes,
                   lista[i].equip->proxima_manutencao);
            temNoventa = true;
        }
    }
    if (!temNoventa)
    {
        printf("  Nenhuma manutencao dentro de 90 dias.\n");
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioManutencoesVencidas(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===             RELATORIO - MANUTENCOES VENCIDAS (URGENTE)           ===\n");
    printf("=========================================================================\n");

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
        printf("Nao ha manutencoes vencidas no momento.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    for (int i = 0; i < totalLista; i++)
    {
        printf("[%-8s] %-20s - Vencida ha %.0f dia%s (prox: %s)\n",
               lista[i].equip->id,
               lista[i].equip->nome,
               -lista[i].diasRestantes,
               (int)(-lista[i].diasRestantes) == 1 ? "" : "s",
               lista[i].equip->proxima_manutencao);
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioManutencoesProximas(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===           RELATORIO - MANUTENCOES PROXIMAS (7/15/30 DIAS)        ===\n");
    printf("=========================================================================\n");

    printf("Escolha o periodo:\n");
    printf("[1] Proximos 7 dias\n");
    printf("[2] Proximos 15 dias\n");
    printf("[3] Proximos 30 dias\n");

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
        printf("Nao ha manutencoes previstas nos proximos %d dias.\n", limiteDias);
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    criterioOrdenacaoSelecionado = '3';
    qsort(lista, totalLista, sizeof(struct EquipamentoView), compararEquipamentos);

    for (int i = 0; i < totalLista; i++)
    {
        printf("[%-8s] %-20s - em %.0f dia%s (%s)\n",
               lista[i].equip->id,
               lista[i].equip->nome,
               lista[i].diasRestantes,
               (int)(lista[i].diasRestantes) == 1 ? "" : "s",
               lista[i].equip->proxima_manutencao);
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioHistoricoManutencoes(void)
{
    printf("Historico de manutencoes: funcionalidade em desenvolvimento.\n");
}

static char selecionarFiltroCategoria(void);
static char selecionarFiltroManutencao(void);
static char selecionarOrdenacaoEquipamentos(void);
static int compararEquipamentosPorNome(const void *a, const void *b);
static int compararEquipamentosPorCategoria(const void *a, const void *b);
static int compararEquipamentosPorData(const void *a, const void *b);

static char filtroCategoriaSelecionado;
static char filtroManutencaoSelecionado;
static char criterioOrdenacaoSelecionado;

static char selecionarFiltroCategoria(void)
{
    printf("Filtrar por categoria? (Digite a letra inicial ou 0 para todas): ");
    char c = lerTecla();
    if (c == '\n' || c == '0')
    {
        return '0';
    }
    return c;
}

static char selecionarFiltroManutencao(void)
{
    printf("\nFiltro de manutencao:\n");
    printf("[0] Todos\n");
    printf("[1] Somente vencidas\n");
    printf("[2] Proximas 7 dias\n");
    printf("[3] Proximas 15 dias\n");
    printf("[4] Proximas 30 dias\n");

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
    printf("\nOrdenacao:\n");
    printf("[1] Nome\n");
    printf("[2] Categoria\n");
    printf("[3] Proxima manutencao\n");

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
