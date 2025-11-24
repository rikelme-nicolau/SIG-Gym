#define _XOPEN_SOURCE 700

#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosGerenciais.h"
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"

/* Relatorios gerenciais: dashboards, analises financeiras, desempenho,
   KPIs, comparativos de planos e rankings Top 10. Usa listas globais
   de alunos/planos/funcionarios/equipamentos carregadas na aplicacao. */

void relatorioTop10Alunos(void);

struct PlanoComparativo
{
    const struct plano *plano;
    int totalAlunos;
    double receitaMensal;
    double ticketMedio;
    double percentualBase;
    int ranking;
};

static char criterioComparativoPlanos = '2';

static char selecionarOrdenacaoComparativo(void);
static int compararPlanosComparativo(const void *a, const void *b);
static int compararPlanosPorNome(const void *a, const void *b);
static int compararPlanosPorAlunos(const void *a, const void *b);
static int compararPlanosPorReceita(const void *a, const void *b);
static int compararPlanosPorValor(const void *a, const void *b);
static int compararPlanosPorPercentual(const void *a, const void *b);
static int compararPlanosPorTotalDesc(const void *a, const void *b);
static int compararAlunosPorId(const void *a, const void *b);
static char selecionarRankingTop10(void);

static int buscarIndicePlanoPorId(const char *id)
{
    if (id == NULL || id[0] == '\0')
    {
        return -1;
    }

    for (int i = 0; i < total_planos; i++)
    {
        if (strcmp(lista_planos[i].id, id) == 0)
        {
            return i;
        }
    }

    return -1;
}

static int extrairHora(const char *horario)
{
    if (horario == NULL || strlen(horario) < 2 || !isdigit((unsigned char)horario[0]))
    {
        return -1;
    }

    if (!isdigit((unsigned char)horario[1]))
    {
        return -1;
    }

    return (horario[0] - '0') * 10 + (horario[1] - '0');
}

static int identificarFaixaHorario(const char *horario)
{
    int hora = extrairHora(horario);

    if (hora < 0 || hora > 23)
    {
        return 4; // Desconhecido
    }

    if (hora >= 5 && hora < 12)
    {
        return 0; // Manha
    }
    if (hora >= 12 && hora < 18)
    {
        return 1; // Tarde
    }
    if (hora >= 18 && hora <= 23)
    {
        return 2; // Noite
    }

    return 3; // Madrugada
}

enum
{
    FAIXA_MANHA = 0,
    FAIXA_TARDE,
    FAIXA_NOITE,
    FAIXA_MADRUGADA,
    FAIXA_DESCONHECIDA,
    TOTAL_FAIXAS
};

static const char *corKpi(double valor)
{
    return valor < 50.0 ? ANSI_RED : "";
}

static const char *resetCorKpi(double valor)
{
    return valor < 50.0 ? ANSI_RESET : "";
}

void moduloRelatoriosGerenciais(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===               RELATORIOS GERENCIAIS - MENU                ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  DASHBOARD EXECUTIVO                                 ===\n");
        printf("===  [2]  ANALISE FINANCEIRA                                  ===\n");
        printf("===  [3]  ANALISE DE DESEMPENHO                               ===\n");
        printf("===  [4]  INDICADORES (KPIs)                                  ===\n");
        printf("===  [5]  COMPARATIVO DE PLANOS                               ===\n");
        printf("===  [6]  TOP 10 (PLANOS / ALUNOS)                            ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

        op = lerTecla();

        limparTela();

        switch (op)
        {
        case '1':
            relatorioDashboardExecutivo();
            break;

        case '2':
            relatorioAnaliseFinanceira();
            break;

        case '3':
            relatorioAnaliseDesempenho();
            break;

        case '4':
            relatorioIndicadoresKPIs();
            break;

        case '5':
            relatorioComparativoPlanos();
            break;

        case '6':
            relatorioTop10Alunos();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (op != '0');
}

static char selecionarRankingTop10(void)
{
    while (1)
    {
        printf("\nEscolha o ranking desejado:\n");
        printf("[1] Top 10 planos com mais alunos\n");
        printf("[2] Top 10 alunos mais antigos (ID menor)\n");
        printf("[3] Distribuicao etaria dos Top 10 alunos mais antigos\n");
        printf("[0] Voltar\n");
        char op = lerTecla();
        if (op >= '0' && op <= '3')
        {
            return op;
        }
        opInvalida();
    }
}

void relatorioComparativoPlanos(void)
{
    limparTela();
    printf("=========================================================================\n");
    printf("===                 RELATORIO - COMPARATIVO DE PLANOS                ===\n");
    printf("=========================================================================\n");

    if (total_planos == 0)
    {
        printf("Nao ha planos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER> para voltar...");
        getchar();
        limparTela();
        return;
    }

    struct PlanoComparativo comparativos[MAX_PLANOS];
    int totalComparados = 0;
    int totalAlunosBase = 0;

    for (int i = 0; i < total_planos && totalComparados < MAX_PLANOS; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int alunosPlano = 0;
        for (int j = 0; j < total_alunos; j++)
        {
            if (!lista_alunos[j].ativo)
            {
                continue;
            }
            if (strcmp(lista_alunos[j].plano_id, lista_planos[i].id) == 0)
            {
                alunosPlano++;
            }
        }

        double receita = alunosPlano * lista_planos[i].valor;
        double ticketMedio = alunosPlano > 0 ? receita / alunosPlano : lista_planos[i].valor;

        comparativos[totalComparados].plano = &lista_planos[i];
        comparativos[totalComparados].totalAlunos = alunosPlano;
        comparativos[totalComparados].receitaMensal = receita;
        comparativos[totalComparados].ticketMedio = ticketMedio;
        comparativos[totalComparados].percentualBase = 0.0;
        comparativos[totalComparados].ranking = 0;
        totalComparados++;
        totalAlunosBase += alunosPlano;
    }

    if (totalComparados == 0)
    {
        printf("Nao ha planos ativos para comparar.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER> para voltar...");
        getchar();
        limparTela();
        return;
    }

    for (int i = 0; i < totalComparados; i++)
    {
        if (totalAlunosBase > 0)
        {
            comparativos[i].percentualBase = (double)comparativos[i].totalAlunos / (double)totalAlunosBase * 100.0;
        }
        else
        {
            comparativos[i].percentualBase = 0.0;
        }
    }

    criterioComparativoPlanos = selecionarOrdenacaoComparativo();
    qsort(comparativos, totalComparados, sizeof(struct PlanoComparativo), compararPlanosComparativo);

    for (int i = 0; i < totalComparados; i++)
    {
        comparativos[i].ranking = i + 1;
    }

    printf("\n+------+----------------------+----------+--------+---------------+---------------+------------+\n");
    printf("| Rank | Plano                | Valor    | Alunos | Receita Mensal| Ticket Medio  | %% Base     |\n");
    printf("+------+----------------------+----------+--------+---------------+---------------+------------+\n");
    for (int i = 0; i < totalComparados; i++)
    {
        const struct PlanoComparativo *p = &comparativos[i];
        printf("| %4d | %-20.20s | %8.2f | %6d | %13.2f | %13.2f | %10.2f%% |\n",
               p->ranking,
               p->plano->nome,
               p->plano->valor,
               p->totalAlunos,
               p->receitaMensal,
               p->ticketMedio,
               p->percentualBase);
    }
    printf("+------+----------------------+----------+--------+---------------+---------------+------------+\n");
    printf("Total de planos comparados: %d | Base de alunos considerada: %d\n", totalComparados, totalAlunosBase);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER> para voltar...");
    getchar();
    limparTela();
}

void relatorioTop10Alunos(void)
{
    while (1)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                   RELATORIO - TOP 10 RANKINGS                    ===\n");
        printf("=========================================================================\n");

        char op = selecionarRankingTop10();
        if (op == '0')
        {
            limparTela();
            return;
        }

        if (op == '1')
        {
            struct
            {
                const struct plano *plano;
                int alunos;
            } ranks[MAX_PLANOS];
            int total = 0;
            for (int i = 0; i < total_planos && total < MAX_PLANOS; i++)
            {
                if (!lista_planos[i].ativo)
                {
                    continue;
                }
                int alunos = 0;
                for (int j = 0; j < total_alunos; j++)
                {
                    if (!lista_alunos[j].ativo)
                    {
                        continue;
                    }
                    if (strcmp(lista_alunos[j].plano_id, lista_planos[i].id) == 0)
                    {
                        alunos++;
                    }
                }
                if (alunos > 0)
                {
                    ranks[total].plano = &lista_planos[i];
                    ranks[total].alunos = alunos;
                    total++;
                }
            }

            qsort(ranks, total, sizeof(ranks[0]), compararPlanosPorTotalDesc);

            int limite = total < 10 ? total : 10;
            printf("\nTop 10 Planos com mais alunos (ativos):\n");
            printf("+------+----------------------+----------+-----------+\n");
            printf("| Rank | Plano                | Alunos   | Valor     |\n");
            printf("+------+----------------------+----------+-----------+\n");
            if (total == 0)
            {
                printf("| %-60s |\n", "Nenhum plano possui alunos ativos.");
            }
            else
            {
                for (int i = 0; i < limite; i++)
                {
                    printf("| %4d | %-20.20s | %8d | %9.2f |\n",
                           i + 1,
                           ranks[i].plano->nome,
                           ranks[i].alunos,
                           ranks[i].plano->valor);
                }
            }
            printf("+------+----------------------+----------+-----------+\n");
            printf("Total de planos com alunos considerados: %d\n", total);
        }
        else if (op == '2' || op == '3')
        {
            const struct aluno *ordenados[MAX_ALUNOS];
            int total = 0;
            for (int i = 0; i < total_alunos && total < MAX_ALUNOS; i++)
            {
                if (!lista_alunos[i].ativo)
                {
                    continue;
                }
                ordenados[total++] = &lista_alunos[i];
            }

            qsort(ordenados, total, sizeof(ordenados[0]), compararAlunosPorId);
            int limite = total < 10 ? total : 10;

            if (op == '2')
            {
                printf("\nTop 10 alunos mais antigos (ID menor):\n");
                printf("+------+---------+----------------------+-------+----------------------+\n");
                printf("| Rank | ID      | Nome                 | Idade | Plano                |\n");
                printf("+------+---------+----------------------+-------+----------------------+\n");
                for (int i = 0; i < limite; i++)
                {
                    const struct aluno *aluno = ordenados[i];
                    int idade = calcularIdade(aluno->idade);
                    int idxPlano = buscarIndicePlanoPorId(aluno->plano_id);
                    const struct plano *plano = idxPlano >= 0 ? &lista_planos[idxPlano] : NULL;
                    printf("| %4d | %-7.7s | %-20.20s | %5d | %-20.20s |\n",
                           i + 1,
                           aluno->id,
                           aluno->nome,
                           idade > 0 ? idade : 0,
                           plano != NULL ? plano->nome : "Sem plano");
                }
                printf("+------+---------+----------------------+-------+----------------------+\n");
                printf("Total de alunos ativos considerados: %d\n", total);
            }
            else
            {
                int faixa18_25 = 0, faixa26_35 = 0, faixa36_45 = 0, faixa46_55 = 0, faixa56 = 0;
                for (int i = 0; i < limite; i++)
                {
                    int idade = calcularIdade(ordenados[i]->idade);
                    if (idade <= 0)
                    {
                        continue;
                    }
                    if (idade >= 18 && idade <= 25)
                        faixa18_25++;
                    else if (idade <= 35)
                        faixa26_35++;
                    else if (idade <= 45)
                        faixa36_45++;
                    else if (idade <= 55)
                        faixa46_55++;
                    else
                        faixa56++;
                }
                printf("\nDistribuicao etaria dos Top 10 alunos mais antigos:\n");
                printf("+--------------+-----------+\n");
                printf("| Faixa        | Quantidade|\n");
                printf("+--------------+-----------+\n");
                printf("| 18-25 anos   | %9d |\n", faixa18_25);
                printf("| 26-35 anos   | %9d |\n", faixa26_35);
                printf("| 36-45 anos   | %9d |\n", faixa36_45);
                printf("| 46-55 anos   | %9d |\n", faixa46_55);
                printf("| 56+ anos     | %9d |\n", faixa56);
                printf("+--------------+-----------+\n");
                printf("Base considerada: %d alunos\n", limite);
            }
        }

        printf("\n=========================================================================\n");
        printf(">>> Pressione <ENTER> para selecionar outro ranking ou voltar...");
        getchar();
    }
}

void relatorioDashboardExecutivo(void)
{
    limparTela();

    int alunosAtivos = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunosAtivos++;
        }
    }
    int alunosInativos = total_alunos - alunosAtivos;
    if (alunosInativos < 0)
    {
        alunosInativos = 0;
    }

    int planosAtivos = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            planosAtivos++;
        }
    }

    int alunosPorPlano[MAX_PLANOS];
    for (int i = 0; i < MAX_PLANOS; i++)
    {
        alunosPorPlano[i] = 0;
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            continue;
        }

        for (int j = 0; j < total_planos; j++)
        {
            if (strcmp(lista_planos[j].id, lista_alunos[i].plano_id) == 0)
            {
                alunosPorPlano[j]++;
                break;
            }
        }
    }

    double receitaMensal = 0.0;
    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }
        receitaMensal += alunosPorPlano[i] * lista_planos[i].valor;
    }

    double taxaRetencao = 0.0;
    if (total_alunos > 0)
    {
        taxaRetencao = ((double)alunosAtivos / (double)total_alunos) * 100.0;
    }

    const char *planoPopular = "Sem alunos vinculados";
    int adesoesPlanoPopular = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (alunosPorPlano[i] > adesoesPlanoPopular)
        {
            adesoesPlanoPopular = alunosPorPlano[i];
            planoPopular = lista_planos[i].nome;
        }
    }

    enum
    {
        TAM_CARGO_RESUMO = 32
    };
    struct CargoResumo
    {
        char cargo[TAM_CARGO_RESUMO];
        int total;
    };
    struct CargoResumo cargosResumo[MAX_FUNCIONARIOS];
    int totalCargosResumo = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (!lista_funcionarios[i].ativo)
        {
            continue;
        }

        char cargoNormalizado[TAM_CARGO_RESUMO];
        if (lista_funcionarios[i].cargo[0] == '\0')
        {
            strncpy(cargoNormalizado, "Nao informado", sizeof(cargoNormalizado));
            cargoNormalizado[sizeof(cargoNormalizado) - 1] = '\0';
        }
        else
        {
            strncpy(cargoNormalizado, lista_funcionarios[i].cargo, sizeof(cargoNormalizado));
            cargoNormalizado[sizeof(cargoNormalizado) - 1] = '\0';
        }

        bool encontrado = false;
        for (int j = 0; j < totalCargosResumo; j++)
        {
            if (strcmp(cargosResumo[j].cargo, cargoNormalizado) == 0)
            {
                cargosResumo[j].total++;
                encontrado = true;
                break;
            }
        }

        if (!encontrado && totalCargosResumo < MAX_FUNCIONARIOS)
        {
            strncpy(cargosResumo[totalCargosResumo].cargo, cargoNormalizado, sizeof(cargosResumo[totalCargosResumo].cargo));
            cargosResumo[totalCargosResumo].cargo[sizeof(cargosResumo[totalCargosResumo].cargo) - 1] = '\0';
            cargosResumo[totalCargosResumo].total = 1;
            totalCargosResumo++;
        }
    }

    const struct equipamento *equipUrgentes[MAX_EQUIPAMENTOS];
    int totalEquipUrgentes = 0;
    time_t agora = time(NULL);

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (!lista_equipamentos[i].ativo)
        {
            continue;
        }

        struct tm tm_manut = {0};
        if (strptime(lista_equipamentos[i].proxima_manutencao, "%d/%m/%Y", &tm_manut) == NULL)
        {
            continue;
        }

        time_t dataManut = mktime(&tm_manut);
        if (dataManut == (time_t)-1)
        {
            continue;
        }

        double diasRestantes = difftime(dataManut, agora) / (60 * 60 * 24);
        if (diasRestantes >= 0.0 && diasRestantes <= 7.0)
        {
            if (totalEquipUrgentes < MAX_EQUIPAMENTOS)
            {
                equipUrgentes[totalEquipUrgentes++] = &lista_equipamentos[i];
            }
        }
    }

    ui_header("Dashboard Executivo", "Visao Geral");
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Alunos: total %d | ativos %d | inativos %d", total_alunos, alunosAtivos, alunosInativos);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Planos ativos: %d", planosAtivos);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Receita mensal estimada: R$ %.2f", receitaMensal);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Taxa de retencao: %.2f %%", taxaRetencao);
    ui_text_line(linha);
    if (adesoesPlanoPopular > 0)
    {
        snprintf(linha, sizeof(linha), "Plano mais popular: %s (%d alunos)", planoPopular, adesoesPlanoPopular);
    }
    else
    {
        snprintf(linha, sizeof(linha), "Plano mais popular: Sem alunos vinculados");
    }
    ui_text_line(linha);

    ui_section_title("Funcionarios por cargo (ativos)");
    if (totalCargosResumo == 0)
    {
        ui_text_line("Nenhum funcionario ativo cadastrado.");
    }
    else
    {
        for (int i = 0; i < totalCargosResumo; i++)
        {
            char cargo_buf[64];
            ui_clip_utf8(cargosResumo[i].cargo, 40, cargo_buf, sizeof(cargo_buf));
            snprintf(linha, sizeof(linha), "- %s: %d", cargo_buf, cargosResumo[i].total);
            ui_text_line(linha);
        }
    }

    ui_section_title("Equipamentos com manutencao nos proximos 7 dias");
    if (totalEquipUrgentes == 0)
    {
        ui_text_line("Nenhum equipamento exige manutencao imediata.");
    }
    else
    {
        for (int i = 0; i < totalEquipUrgentes; i++)
        {
            char nome_buf[64];
            ui_clip_utf8(equipUrgentes[i]->nome, 30, nome_buf, sizeof(nome_buf));
            snprintf(linha, sizeof(linha), "- [%.12s] %-24s ate %.12s",
                     equipUrgentes[i]->id,
                     nome_buf,
                     equipUrgentes[i]->proxima_manutencao);
            ui_text_line(linha);
        }
    }

    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

static char selecionarOrdenacaoComparativo(void)
{
    while (1)
    {
        printf("\nSelecione o criterio de ordenacao:\n");
        printf("[1] Nome do plano (A-Z)\n");
        printf("[2] Total de alunos (maior primeiro)\n");
        printf("[3] Receita mensal (maior primeiro)\n");
        printf("[4] Valor do plano (maior primeiro)\n");
        printf("[5] Percentual da base (maior primeiro)\n");

        char op = lerTecla();
        if (op >= '1' && op <= '5')
        {
            return op;
        }

        opInvalida();
    }
}

static int compararPlanosComparativo(const void *a, const void *b)
{
    int cmp = 0;
    switch (criterioComparativoPlanos)
    {
    case '1':
        cmp = compararPlanosPorNome(a, b);
        break;
    case '3':
        cmp = compararPlanosPorReceita(a, b);
        break;
    case '4':
        cmp = compararPlanosPorValor(a, b);
        break;
    case '5':
        cmp = compararPlanosPorPercentual(a, b);
        break;
    case '2':
    default:
        cmp = compararPlanosPorAlunos(a, b);
        break;
    }

    return cmp;
}

static int compararPlanosPorNome(const void *a, const void *b)
{
    const struct PlanoComparativo *pa = (const struct PlanoComparativo *)a;
    const struct PlanoComparativo *pb = (const struct PlanoComparativo *)b;

    int cmp = strcmp(pa->plano->nome, pb->plano->nome);
    if (cmp == 0)
    {
        cmp = strcmp(pa->plano->id, pb->plano->id);
    }
    return cmp;
}

static int compararPlanosPorAlunos(const void *a, const void *b)
{
    const struct PlanoComparativo *pa = (const struct PlanoComparativo *)a;
    const struct PlanoComparativo *pb = (const struct PlanoComparativo *)b;

    int cmp = pb->totalAlunos - pa->totalAlunos; // decrescente
    if (cmp == 0)
    {
        cmp = compararPlanosPorNome(a, b);
    }
    return cmp;
}

static int compararPlanosPorReceita(const void *a, const void *b)
{
    const struct PlanoComparativo *pa = (const struct PlanoComparativo *)a;
    const struct PlanoComparativo *pb = (const struct PlanoComparativo *)b;

    int cmp = (pb->receitaMensal > pa->receitaMensal) - (pb->receitaMensal < pa->receitaMensal); // decrescente
    if (cmp == 0)
    {
        cmp = compararPlanosPorNome(a, b);
    }
    return cmp;
}

static int compararPlanosPorValor(const void *a, const void *b)
{
    const struct PlanoComparativo *pa = (const struct PlanoComparativo *)a;
    const struct PlanoComparativo *pb = (const struct PlanoComparativo *)b;

    int cmp = (pb->ticketMedio > pa->ticketMedio) - (pb->ticketMedio < pa->ticketMedio); // decrescente
    if (cmp == 0)
    {
        cmp = compararPlanosPorNome(a, b);
    }
    return cmp;
}

static int compararPlanosPorPercentual(const void *a, const void *b)
{
    const struct PlanoComparativo *pa = (const struct PlanoComparativo *)a;
    const struct PlanoComparativo *pb = (const struct PlanoComparativo *)b;

    int cmp = (pb->percentualBase > pa->percentualBase) - (pb->percentualBase < pa->percentualBase); // decrescente
    if (cmp == 0)
    {
        cmp = compararPlanosPorNome(a, b);
    }
    return cmp;
}

static int compararPlanosPorTotalDesc(const void *a, const void *b)
{
    const struct
    {
        const struct plano *plano;
        int alunos;
    } *pa = a, *pb = b;

    int cmp = pb->alunos - pa->alunos; // decrescente
    if (cmp == 0)
    {
        cmp = strcmp(pa->plano->nome, pb->plano->nome);
    }
    if (cmp == 0)
    {
        cmp = strcmp(pa->plano->id, pb->plano->id);
    }
    return cmp;
}

static int compararAlunosPorId(const void *a, const void *b)
{
    const struct aluno *const *aa = (const struct aluno *const *)a;
    const struct aluno *const *bb = (const struct aluno *const *)b;
    const struct aluno *alunoA = *aa;
    const struct aluno *alunoB = *bb;
    int cmp = strcmp(alunoA->id, alunoB->id);
    if (cmp == 0)
    {
        cmp = strcmp(alunoA->nome, alunoB->nome);
    }
    return cmp;
}
void relatorioAnaliseFinanceira(void)
{
    limparTela();

    if (total_planos == 0)
    {
        printf("=========================================================================\n");
        printf("===                 ANALISE FINANCEIRA - PLANOS                      ===\n");
        printf("=========================================================================\n");
        printf("Nenhum plano cadastrado no sistema.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    int alunosPorPlano[MAX_PLANOS];
    for (int i = 0; i < MAX_PLANOS; i++)
    {
        alunosPorPlano[i] = 0;
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            continue;
        }

        for (int j = 0; j < total_planos; j++)
        {
            if (strcmp(lista_planos[j].id, lista_alunos[i].plano_id) == 0)
            {
                alunosPorPlano[j]++;
                break;
            }
        }
    }

    double receitaTotal = 0.0;
    double ticketMedio = 0.0;
    double maiorReceita = -1.0;
    double menorReceita = DBL_MAX;
    const char *planoMaiorReceita = "N/A";
    const char *planoMenorReceita = "N/A";
    bool possuiPlanoAtivo = false;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        possuiPlanoAtivo = true;
        double receitaPlano = alunosPorPlano[i] * lista_planos[i].valor;
        receitaTotal += receitaPlano;

        if (receitaPlano > maiorReceita)
        {
            maiorReceita = receitaPlano;
            planoMaiorReceita = lista_planos[i].nome;
        }

        if (receitaPlano < menorReceita)
        {
            menorReceita = receitaPlano;
            planoMenorReceita = lista_planos[i].nome;
        }
    }

    if (!possuiPlanoAtivo)
    {
        printf("=========================================================================\n");
        printf("===                 ANALISE FINANCEIRA - PLANOS                      ===\n");
        printf("=========================================================================\n");
        printf("Nao ha planos ativos para gerar receitas.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    int alunosComPlano = 0;
    for (int i = 0; i < MAX_PLANOS; i++)
    {
        alunosComPlano += alunosPorPlano[i];
    }

    if (alunosComPlano > 0)
    {
        ticketMedio = receitaTotal / (double)alunosComPlano;
    }

    printf("=========================================================================\n");
    printf("===                    ANALISE FINANCEIRA - RECEITAS                ===\n");
    printf("=========================================================================\n");
    printf("Receita mensal total estimada: R$ %10.2f\n", receitaTotal);
    printf("Ticket medio (por aluno): R$ %10.2f\n", ticketMedio);
    printf("Plano com maior faturamento : %s (R$ %.2f)\n", planoMaiorReceita, maiorReceita);
    printf("Plano com menor faturamento : %s (R$ %.2f)\n", planoMenorReceita, menorReceita);
    printf("-------------------------------------------------------------------------\n");
    printf("+------------------------------+--------+------------+---------------+\n");
    printf("| %-28s | %-6s | %-10s | %-13s |\n", "Plano", "Alunos", "Valor Unit.", "Receita Total");
    printf("+------------------------------+--------+------------+---------------+\n");

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        double receitaPlano = alunosPorPlano[i] * lista_planos[i].valor;
        printf("| %-28.28s | %6d | R$ %8.2f | R$ %10.2f |\n",
               lista_planos[i].nome,
               alunosPorPlano[i],
               lista_planos[i].valor,
               receitaPlano);
    }

    printf("+------------------------------+--------+------------+---------------+\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAnaliseDesempenho(void)
{
    limparTela();

    int alunosAtivos = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunosAtivos++;
        }
    }

    if (alunosAtivos == 0)
    {
        printf("=========================================================================\n");
        printf("===                 ANALISE DE DESEMPENHO - METRICAS                 ===\n");
        printf("=========================================================================\n");
        printf("Nao ha alunos ativos para gerar o relatorio.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    int alunosPorPlano[MAX_PLANOS];
    for (int i = 0; i < MAX_PLANOS; i++)
    {
        alunosPorPlano[i] = 0;
    }

    int distribuicaoFaixa[TOTAL_FAIXAS];
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        distribuicaoFaixa[i] = 0;
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        int indicePlano = buscarIndicePlanoPorId(lista_alunos[i].plano_id);
        if (indicePlano < 0)
        {
            continue;
        }

        alunosPorPlano[indicePlano]++;

        int faixa = identificarFaixaHorario(lista_planos[indicePlano].horario_inicio);
        if (faixa < 0 || faixa >= TOTAL_FAIXAS)
        {
            faixa = FAIXA_DESCONHECIDA;
        }
        distribuicaoFaixa[faixa]++;
    }

    struct PlanoRanking
    {
        const struct plano *plano;
        int alunos;
        double percentual;
    };

    struct PlanoRanking ranking[MAX_PLANOS];
    int totalRanking = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        ranking[totalRanking].plano = &lista_planos[i];
        ranking[totalRanking].alunos = alunosPorPlano[i];
        ranking[totalRanking].percentual = alunosAtivos > 0 ? ((double)alunosPorPlano[i] / alunosAtivos) * 100.0 : 0.0;
        totalRanking++;
    }

    for (int i = 0; i < totalRanking - 1; i++)
    {
        for (int j = i + 1; j < totalRanking; j++)
        {
            if (ranking[j].alunos > ranking[i].alunos)
            {
                struct PlanoRanking tmp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = tmp;
            }
        }
    }

    double capacidadeTeorica = MAX_ALUNOS;
    double taxaOcupacao = (capacidadeTeorica > 0.0) ? ((double)alunosAtivos / capacidadeTeorica) * 100.0 : 0.0;

    const char *labelsFaixa[TOTAL_FAIXAS] = {
        "Manha (05h-11h)",
        "Tarde (12h-17h)",
        "Noite (18h-23h)",
        "Madrugada (00h-04h)",
        "Nao definido"};

    printf("=========================================================================\n");
    printf("===                ANALISE DE DESEMPENHO - OPERACIONAL               ===\n");
    printf("=========================================================================\n");
    printf("Taxa de ocupacao geral: %6.2f %% (Alunos ativos: %d / Capacidade: %.0f)\n",
           taxaOcupacao, alunosAtivos, capacidadeTeorica);
    printf("-------------------------------------------------------------------------\n");
    printf("RANKING DE PLANOS POR POPULARIDADE\n");
    printf("+----+------------------------------+--------+-----------+\n");
    printf("| #  | Plano                        | Alunos | %% Base   |\n");
    printf("+----+------------------------------+--------+-----------+\n");

    if (totalRanking == 0)
    {
        printf("| -- | Nenhum plano ativo cadastrado.                       |\n");
    }
    else
    {
        for (int i = 0; i < totalRanking; i++)
        {
            printf("| %2d | %-28.28s | %6d | %8.2f |\n",
                   i + 1,
                   ranking[i].plano->nome,
                   ranking[i].alunos,
                   ranking[i].percentual);
        }
    }

    printf("+----+------------------------------+--------+-----------+\n");
    printf("-------------------------------------------------------------------------\n");
    printf("DISTRIBUICAO DE ALUNOS POR FAIXA HORARIA DO PLANO\n");
    printf("+-------------------------+--------+-----------+\n");
    printf("| Faixa                   | Alunos | %% Sobre  |\n");
    printf("+-------------------------+--------+-----------+\n");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentualFaixa = alunosAtivos > 0 ? ((double)distribuicaoFaixa[i] / alunosAtivos) * 100.0 : 0.0;
        printf("| %-23.23s | %6d | %8.2f |\n",
               labelsFaixa[i],
               distribuicaoFaixa[i],
               percentualFaixa);
    }
    printf("+-------------------------+--------+-----------+\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioIndicadoresKPIs(void)
{
    limparTela();

    if (total_alunos == 0)
    {
        printf("=========================================================================\n");
        printf("===                  INDICADORES - KPIs CRITICOS                     ===\n");
        printf("=========================================================================\n");
        printf("Nao ha alunos cadastrados para calcular os indicadores.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    int alunosAtivos = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunosAtivos++;
        }
    }
    int alunosInativos = total_alunos - alunosAtivos;
    if (alunosInativos < 0)
    {
        alunosInativos = 0;
    }

    int alunosPorPlano[MAX_PLANOS];
    for (int i = 0; i < MAX_PLANOS; i++)
    {
        alunosPorPlano[i] = 0;
    }

    int distribuicaoFaixa[TOTAL_FAIXAS];
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        distribuicaoFaixa[i] = 0;
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        int indicePlano = buscarIndicePlanoPorId(lista_alunos[i].plano_id);
        if (indicePlano < 0)
        {
            continue;
        }

        alunosPorPlano[indicePlano]++;
        int faixa = identificarFaixaHorario(lista_planos[indicePlano].horario_inicio);
        if (faixa < 0 || faixa >= TOTAL_FAIXAS)
        {
            faixa = FAIXA_DESCONHECIDA;
        }
        distribuicaoFaixa[faixa]++;
    }

    double receitaTotal = 0.0;
    int planosAtivos = 0;
    int somaAtividades = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        planosAtivos++;
        somaAtividades += lista_planos[i].total_atividades;
        receitaTotal += alunosPorPlano[i] * lista_planos[i].valor;
    }

    double taxaRetencao = total_alunos > 0 ? ((double)alunosAtivos / total_alunos) * 100.0 : 0.0;
    double taxaCancelamento = total_alunos > 0 ? ((double)alunosInativos / total_alunos) * 100.0 : 0.0;
    double ticketMedio = alunosAtivos > 0 ? receitaTotal / alunosAtivos : 0.0;
    double receitaMediaPlano = planosAtivos > 0 ? receitaTotal / planosAtivos : 0.0;
    double atividadesMedias = planosAtivos > 0 ? (double)somaAtividades / planosAtivos : 0.0;
    double taxaOcupacaoGeral = MAX_ALUNOS > 0 ? ((double)alunosAtivos / MAX_ALUNOS) * 100.0 : 0.0;

    const char *labelsFaixa[TOTAL_FAIXAS] = {
        "Manha (05h-11h)",
        "Tarde (12h-17h)",
        "Noite (18h-23h)",
        "Madrugada (00h-04h)",
        "Nao definido"};

    printf("=========================================================================\n");
    printf("===                     INDICADORES DE PERFORMANCE                   ===\n");
    printf("=========================================================================\n");
    printf("%sTaxa de retencao:%s      %5.1f %%\n",
           corKpi(taxaRetencao),
           resetCorKpi(taxaRetencao),
           taxaRetencao);
    printf("%sTaxa de cancelamento:%s %5.1f %%\n",
           corKpi(taxaCancelamento),
           resetCorKpi(taxaCancelamento),
           taxaCancelamento);
    printf("%sTaxa de ocupacao geral:%s %5.1f %%\n",
           corKpi(taxaOcupacaoGeral),
           resetCorKpi(taxaOcupacaoGeral),
           taxaOcupacaoGeral);
    printf("Ticket medio:            R$ %8.2f\n", ticketMedio);
    printf("Receita media por plano: R$ %8.2f\n", receitaMediaPlano);
    printf("Media de atividades por plano: %.2f\n", atividadesMedias);
    printf("-------------------------------------------------------------------------\n");
    printf("TAXA DE OCUPACAO POR HORARIO\n");
    printf("+-------------------------+--------+-----------+\n");
    printf("| Faixa                   | Alunos | Taxa (%)  |\n");
    printf("+-------------------------+--------+-----------+\n");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentualFaixa = alunosAtivos > 0 ? ((double)distribuicaoFaixa[i] / alunosAtivos) * 100.0 : 0.0;
        printf("| %-23.23s | %6d | %s%8.1f%s |\n",
               labelsFaixa[i],
               distribuicaoFaixa[i],
               corKpi(percentualFaixa),
               percentualFaixa,
               resetCorKpi(percentualFaixa));
    }
    printf("+-------------------------+--------+-----------+\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}
