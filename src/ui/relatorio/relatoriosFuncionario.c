#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosFuncionario.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/aluno/cadastrarAluno.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Relatorios de funcionarios: listagens, agrupamentos por cargo,
   distribuicao etaria e analises simples da equipe. */

#define LIST_COL_ID 7
#define LIST_COL_NOME 18
#define LIST_COL_CARGO 15
#define LIST_COL_IDADE 5
#define LIST_COL_TEL 15

#define CARGO_COL_NOME 22
#define CARGO_COL_QTD 8
#define CARGO_COL_MEDIA 10
#define CARGO_COL_RANGE 18

static char criterioOrdenacaoFuncionario;

static void relatorioListagemFuncionarios(void);
static void relatorioFuncionariosPorCargo(void);
static void relatorioDistribuicaoFaixaEtaria(void);
static void relatorioAnaliseEquipe(void);
static char selecionarFiltroStatusFuncionario(void);
static void solicitarFiltroCargo(char *dest, size_t tamanho);
static char selecionarOrdenacaoFuncionario(void);
static int compararFuncionarios(const void *a, const void *b);
static void cabecalho_relatorio(const char *subtitulo);
static void aguardar_voltar(void);
static void tabela_listagem_header(void);
static void tabela_listagem_row(const struct funcionario *f);
static void tabela_cargo_header(void);
static void tabela_cargo_row(const char *nome, int total, double media, int minIdade, int maxIdade);

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

static void tabela_listagem_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", LIST_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", LIST_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Cargo", LIST_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Idade", LIST_COL_IDADE);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Telefone", LIST_COL_TEL);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_listagem_row(const struct funcionario *f)
{
    if (f == NULL)
    {
        return;
    }
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[32];
    ui_clip_utf8(f->nome, 18, nomeClip, sizeof(nomeClip));
    char cargoClip[28];
    ui_clip_utf8(f->cargo, 15, cargoClip, sizeof(cargoClip));
    char idadeStr[8];
    snprintf(idadeStr, sizeof(idadeStr), "%d", f->idade);
    char telClip[24];
    ui_clip_utf8(f->telefone, 15, telClip, sizeof(telClip));

    ui_append_col(linha, sizeof(linha), &pos, f->id, LIST_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nomeClip, LIST_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, cargoClip, LIST_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, idadeStr, LIST_COL_IDADE);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, telClip, LIST_COL_TEL);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_cargo_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Cargo", CARGO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Qtd", CARGO_COL_QTD);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Idade media", CARGO_COL_MEDIA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Min/Max", CARGO_COL_RANGE);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_cargo_row(const char *nome, int total, double media, int minIdade, int maxIdade)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    char nomeClip[40];
    ui_clip_utf8(nome != NULL ? nome : "N/A", 22, nomeClip, sizeof(nomeClip));
    char qtd[12];
    snprintf(qtd, sizeof(qtd), "%d", total);
    char mediaStr[16];
    snprintf(mediaStr, sizeof(mediaStr), "%.1f", media);
    char range[24];
    if (minIdade >= 0 && maxIdade >= 0)
    {
        snprintf(range, sizeof(range), "%d - %d", minIdade, maxIdade);
    }
    else
    {
        strncpy(range, "N/A", sizeof(range));
        range[sizeof(range) - 1] = '\0';
    }

    ui_append_col(linha, sizeof(linha), &pos, nomeClip, CARGO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, qtd, CARGO_COL_QTD);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, mediaStr, CARGO_COL_MEDIA);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, range, CARGO_COL_RANGE);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void moduloRelatoriosFuncionario(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios de Funcionarios");
        ui_empty_line();
        ui_menu_option('1', "Listagem completa de funcionarios");
        ui_menu_option('2', "Funcionarios por cargo");
        ui_menu_option('3', "Distribuicao por faixa etaria");
        ui_menu_option('4', "Analise de equipe");
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
            relatorioListagemFuncionarios();
            break;

        case '2':
            relatorioFuncionariosPorCargo();
            break;

        case '3':
            relatorioDistribuicaoFaixaEtaria();
            break;

        case '4':
            relatorioAnaliseEquipe();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

static void relatorioListagemFuncionarios(void)
{
    cabecalho_relatorio("Relatorio - Listagem de funcionarios");

    if (total_funcionarios == 0)
    {
        ui_center_text("Nao ha funcionarios cadastrados.");
        aguardar_voltar();
        return;
    }

    char filtroStatus = selecionarFiltroStatusFuncionario();
    char cargoFiltro[64];
    solicitarFiltroCargo(cargoFiltro, sizeof(cargoFiltro));
    criterioOrdenacaoFuncionario = selecionarOrdenacaoFuncionario();

    const struct funcionario *lista[MAX_FUNCIONARIOS];
    int totalLista = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (filtroStatus == '1' && !lista_funcionarios[i].ativo)
        {
            continue;
        }
        if (filtroStatus == '2' && lista_funcionarios[i].ativo)
        {
            continue;
        }
        if (cargoFiltro[0] != '\0' &&
            strncasecmp(lista_funcionarios[i].cargo, cargoFiltro, strlen(cargoFiltro)) != 0)
        {
            continue;
        }
        lista[totalLista++] = &lista_funcionarios[i];
    }

    if (totalLista == 0)
    {
        ui_section_title("Nenhum funcionario encontrado");
        ui_text_line("Ajuste os filtros e tente novamente.");
        aguardar_voltar();
        return;
    }

    qsort(lista, totalLista, sizeof(const struct funcionario *), compararFuncionarios);

    cabecalho_relatorio("Relatorio - Listagem de funcionarios");
    ui_section_title("Filtros aplicados");
    char linha[UI_INNER + 1];
    const char *statusDesc = filtroStatus == '1' ? "Somente ativos" : (filtroStatus == '2' ? "Somente inativos" : "Todos");
    snprintf(linha, sizeof(linha), "Status: %s", statusDesc);
    ui_text_line(linha);
    char cargoCurto[32];
    const char *cargoOrigem = cargoFiltro[0] != '\0' ? cargoFiltro : "Todos";
    ui_clip_utf8(cargoOrigem, 22, cargoCurto, sizeof(cargoCurto));
    snprintf(linha, sizeof(linha), "Cargo: %s", cargoCurto);
    ui_text_line(linha);
    const char *ordDesc;
    switch (criterioOrdenacaoFuncionario)
    {
    case '2':
        ordDesc = "Cargo";
        break;
    case '3':
        ordDesc = "Idade";
        break;
    case '4':
        ordDesc = "Cargo + Nome";
        break;
    case '5':
        ordDesc = "Idade (crescente)";
        break;
    case '6':
        ordDesc = "Idade (decrescente)";
        break;
    case '1':
    default:
        ordDesc = "Nome";
        break;
    }
    snprintf(linha, sizeof(linha), "Ordenacao: %s", ordDesc);
    ui_text_line(linha);
    ui_line('-');

    tabela_listagem_header();
    for (int i = 0; i < totalLista; i++)
    {
        tabela_listagem_row(lista[i]);
    }
    ui_line('-');
    snprintf(linha, sizeof(linha), "Total listado: %d", totalLista);
    ui_text_line(linha);
    ui_text_line("Sugestao: revise contatos e distribuicao de cargos.");
    ui_line('-');

    aguardar_voltar();
}

static void relatorioFuncionariosPorCargo(void)
{
    cabecalho_relatorio("Relatorio - Funcionarios por cargo");

    if (total_funcionarios == 0)
    {
        ui_center_text("Nao ha funcionarios cadastrados.");
        aguardar_voltar();
        return;
    }

    struct CargoView
    {
        char nome[64];
        const struct funcionario *lista[MAX_FUNCIONARIOS];
        int total;
        int somaIdade;
    };

    struct CargoView cargos[MAX_FUNCIONARIOS];
    int totalCargos = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].cargo[0] == '\0')
        {
            continue;
        }

        int indice = -1;
        for (int c = 0; c < totalCargos; c++)
        {
            if (strcasecmp(cargos[c].nome, lista_funcionarios[i].cargo) == 0)
            {
                indice = c;
                break;
            }
        }

        if (indice == -1 && totalCargos < MAX_FUNCIONARIOS)
        {
            strncpy(cargos[totalCargos].nome, lista_funcionarios[i].cargo, sizeof(cargos[totalCargos].nome));
            cargos[totalCargos].nome[sizeof(cargos[totalCargos].nome) - 1] = '\0';
            cargos[totalCargos].total = 0;
            cargos[totalCargos].somaIdade = 0;
            indice = totalCargos;
            totalCargos++;
        }

        if (indice != -1)
        {
            cargos[indice].lista[cargos[indice].total++] = &lista_funcionarios[i];
            cargos[indice].somaIdade += lista_funcionarios[i].idade;
        }
    }

    if (totalCargos == 0)
    {
        ui_section_title("Nenhum cargo informado");
        ui_text_line("Nao foi possivel agrupar funcionarios por cargo.");
        aguardar_voltar();
        return;
    }

    for (int i = 0; i < totalCargos - 1; i++)
    {
        for (int j = i + 1; j < totalCargos; j++)
        {
            if (strcasecmp(cargos[j].nome, cargos[i].nome) < 0)
            {
                struct CargoView tmp = cargos[i];
                cargos[i] = cargos[j];
                cargos[j] = tmp;
            }
        }
    }

    tabela_cargo_header();
    for (int i = 0; i < totalCargos; i++)
    {
        double media = cargos[i].total > 0 ? (double)cargos[i].somaIdade / cargos[i].total : 0.0;
        const struct funcionario *maisNovo = cargos[i].lista[0];
        const struct funcionario *maisVelho = cargos[i].lista[0];

        for (int j = 1; j < cargos[i].total; j++)
        {
            if (cargos[i].lista[j]->idade < maisNovo->idade)
            {
                maisNovo = cargos[i].lista[j];
            }
            if (cargos[i].lista[j]->idade > maisVelho->idade)
            {
                maisVelho = cargos[i].lista[j];
            }
        }

        tabela_cargo_row(cargos[i].nome, cargos[i].total, media, maisNovo->idade, maisVelho->idade);
    }
    ui_line('-');
    ui_text_line("Dica: avalie distribuicao de senioridade por cargo.");
    ui_line('-');
    aguardar_voltar();
}

static void relatorioDistribuicaoFaixaEtaria(void)
{
    cabecalho_relatorio("Relatorio - Faixa etaria (funcionarios)");

    if (total_funcionarios == 0)
    {
        ui_center_text("Nao ha funcionarios cadastrados.");
        aguardar_voltar();
        return;
    }

    enum
    {
        FAIXA_18_25 = 0,
        FAIXA_26_35,
        FAIXA_36_45,
        FAIXA_46_55,
        FAIXA_56_MAIS,
        TOTAL_FAIXAS
    };

    const char *labels[TOTAL_FAIXAS] = {
        "18-25 anos",
        "26-35 anos",
        "36-45 anos",
        "46-55 anos",
        "56+ anos"};

    const struct funcionario *faixas[TOTAL_FAIXAS][MAX_FUNCIONARIOS];
    int contadores[TOTAL_FAIXAS] = {0};
    int somaIdades = 0;
    int totalConsiderados = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        int idade = lista_funcionarios[i].idade;
        if (idade <= 0)
        {
            continue;
        }

        int faixa = FAIXA_56_MAIS;
        if (idade >= 18 && idade <= 25)
        {
            faixa = FAIXA_18_25;
        }
        else if (idade <= 35)
        {
            faixa = FAIXA_26_35;
        }
        else if (idade <= 45)
        {
            faixa = FAIXA_36_45;
        }
        else if (idade <= 55)
        {
            faixa = FAIXA_46_55;
        }

        faixas[faixa][contadores[faixa]++] = &lista_funcionarios[i];
        somaIdades += idade;
        totalConsiderados++;
    }

    if (totalConsiderados == 0)
    {
        ui_section_title("Dados insuficientes");
        ui_text_line("Nao foi possivel calcular idades validas para os funcionarios.");
        aguardar_voltar();
        return;
    }

    int faixaPredominante = 0;
    for (int i = 1; i < TOTAL_FAIXAS; i++)
    {
        if (contadores[i] > contadores[faixaPredominante])
        {
            faixaPredominante = i;
        }
    }

    double idadeMedia = (double)somaIdades / totalConsiderados;
    double percentualPredominante = (double)contadores[faixaPredominante] / totalConsiderados * 100.0;
    bool desequilibrio = percentualPredominante > 60.0;

    ui_section_title("Distribuicao por faixa");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentual = (double)contadores[i] / totalConsiderados * 100.0;
        char linha[UI_INNER + 1];
        snprintf(linha, sizeof(linha), "%-12s: %3d func (%.2f%%)",
                 labels[i],
                 contadores[i],
                 percentual);
        ui_text_line(linha);
        if (contadores[i] > 0)
        {
            for (int j = 0; j < contadores[i]; j++)
            {
                char nomeClip[48];
                ui_clip_utf8(faixas[i][j]->nome, 40, nomeClip, sizeof(nomeClip));
                char item[UI_INNER + 1];
                snprintf(item, sizeof(item), "  - %s (%d anos)", nomeClip, faixas[i][j]->idade);
                ui_text_line(item);
            }
        }
    }

    char resumo[UI_INNER + 1];
    snprintf(resumo, sizeof(resumo), "Faixa predominante: %s (%.2f%%)", labels[faixaPredominante], percentualPredominante);
    ui_line('-');
    ui_text_line(resumo);
    snprintf(resumo, sizeof(resumo), "Idade media da equipe: %.1f anos", idadeMedia);
    ui_text_line(resumo);
    ui_text_line(desequilibrio ? "Aviso: concentracao alta em uma faixa." : "Distribuicao etaria equilibrada.");
    snprintf(resumo, sizeof(resumo), "Total considerado: %d funcionario%s", totalConsiderados, totalConsiderados == 1 ? "" : "s");
    ui_text_line(resumo);
    ui_line('-');
    aguardar_voltar();
}

static void relatorioAnaliseEquipe(void)
{
    cabecalho_relatorio("Relatorio - Analise de equipe");

    if (total_funcionarios == 0)
    {
        ui_center_text("Nao ha funcionarios cadastrados.");
        aguardar_voltar();
        return;
    }

    int ativos = 0;
    int inativos = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            ativos++;
        }
        else
        {
            inativos++;
        }
    }

    struct CargoAnalise
    {
        char nome[64];
        int total;
        int somaIdade;
    };

    struct CargoAnalise cargos[MAX_FUNCIONARIOS];
    int totalCargos = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].cargo[0] == '\0')
        {
            continue;
        }

        int indice = -1;
        for (int c = 0; c < totalCargos; c++)
        {
            if (strcasecmp(cargos[c].nome, lista_funcionarios[i].cargo) == 0)
            {
                indice = c;
                break;
            }
        }

        if (indice == -1 && totalCargos < MAX_FUNCIONARIOS)
        {
            strncpy(cargos[totalCargos].nome, lista_funcionarios[i].cargo, sizeof(cargos[totalCargos].nome));
            cargos[totalCargos].nome[sizeof(cargos[totalCargos].nome) - 1] = '\0';
            cargos[totalCargos].total = 0;
            cargos[totalCargos].somaIdade = 0;
            indice = totalCargos;
            totalCargos++;
        }

        if (indice != -1)
        {
            cargos[indice].total++;
            cargos[indice].somaIdade += lista_funcionarios[i].idade;
        }
    }

    ui_section_title("Status geral");
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Ativos: %d | Inativos: %d", ativos, inativos);
    ui_text_line(linha);
    double taxaRetencao = total_funcionarios > 0 ? ((double)ativos / total_funcionarios) * 100.0 : 0.0;
    snprintf(linha, sizeof(linha), "Taxa de retencao: %.2f%%", taxaRetencao);
    ui_text_line(linha);
    ui_line('-');

    if (totalCargos > 0)
    {
        ui_section_title("Distribuicao por cargo");
        tabela_cargo_header();
        for (int i = 0; i < totalCargos; i++)
        {
            double media = cargos[i].total > 0 ? (double)cargos[i].somaIdade / cargos[i].total : 0.0;
            tabela_cargo_row(cargos[i].nome, cargos[i].total, media, -1, -1);
        }
        ui_line('-');
    }

    ui_text_line("Revise gaps de cargos e distribuicao de senioridade.");
    ui_line('-');
    aguardar_voltar();
}

static char selecionarFiltroStatusFuncionario(void)
{
    cabecalho_relatorio("Filtro - Status dos funcionarios");
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
    return selecionarFiltroStatusFuncionario();
}

static void solicitarFiltroCargo(char *dest, size_t tamanho)
{
    cabecalho_relatorio("Filtro - Cargo");
    ui_text_line("Digite o cargo para filtrar (ENTER para todos).");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
    if (fgets(dest, tamanho, stdin) != NULL)
    {
        dest[strcspn(dest, "\n")] = '\0';
    }
}

static char selecionarOrdenacaoFuncionario(void)
{
    while (1)
    {
        cabecalho_relatorio("Filtro - Ordenacao de funcionarios");
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Cargo");
        ui_menu_option('3', "Idade");
        ui_menu_option('4', "Cargo + Nome");
        ui_menu_option('5', "Idade (crescente)");
        ui_menu_option('6', "Idade (decrescente)");
        ui_section_title("Escolha uma opcao");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

        char op = lerTecla();
        if (op >= '1' && op <= '6')
        {
            return op;
        }

        opInvalida();
    }
}

static int compararFuncionarios(const void *a, const void *b)
{
    const struct funcionario *fa = *(const struct funcionario **)a;
    const struct funcionario *fb = *(const struct funcionario **)b;

    switch (criterioOrdenacaoFuncionario)
    {
    case '2':
        return strcasecmp(fa->cargo, fb->cargo);
    case '3':
        return fa->idade - fb->idade;
    case '4':
    {
        int cmpCargo = strcasecmp(fa->cargo, fb->cargo);
        if (cmpCargo != 0)
        {
            return cmpCargo;
        }
        return strcasecmp(fa->nome, fb->nome);
    }
    case '5':
        return fa->idade - fb->idade;
    case '6':
        return fb->idade - fa->idade;
    case '1':
    default:
        return strcasecmp(fa->nome, fb->nome);
    }
}
