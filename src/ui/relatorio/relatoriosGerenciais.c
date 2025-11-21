#define _XOPEN_SOURCE 700

#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosGerenciais.h"
#include "ui/utils/lerTecla.h"

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"

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

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (op != '0');
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

    printf("=========================================================================\n");
    printf("===                 DASHBOARD EXECUTIVO - VISAO GERAL                ===\n");
    printf("=========================================================================\n");
    printf("===  ALUNOS: TOTAL %4d | ATIVOS %4d | INATIVOS %4d                   ===\n", total_alunos, alunosAtivos, alunosInativos);
    printf("===  PLANOS ATIVOS: %4d                                            ===\n", planosAtivos);
    printf("===  RECEITA MENSAL ESTIMADA: R$ %10.2f                          ===\n", receitaMensal);
    printf("===  TAXA DE RETENCAO: %6.2f %%                                      ===\n", taxaRetencao);
    if (adesoesPlanoPopular > 0)
    {
        printf("===  PLANO MAIS POPULAR: %-30.30s (%2d alunos)        ===\n", planoPopular, adesoesPlanoPopular);
    }
    else
    {
        printf("===  PLANO MAIS POPULAR: Sem alunos vinculados                  ===\n");
    }
    printf("=========================================================================\n");
    printf("===                FUNCIONARIOS POR CARGO (ATIVOS)                 ===\n");
    if (totalCargosResumo == 0)
    {
        printf("===    Nenhum funcionario ativo cadastrado.                        ===\n");
    }
    else
    {
        for (int i = 0; i < totalCargosResumo; i++)
        {
            printf("===    - %-20.20s : %3d                                    ===\n", cargosResumo[i].cargo, cargosResumo[i].total);
        }
    }
    printf("=========================================================================\n");
    printf("===        EQUIPAMENTOS COM MANUTENCAO NOS PROXIMOS 7 DIAS         ===\n");
    if (totalEquipUrgentes == 0)
    {
        printf("===    Nenhum equipamento exige manutencao imediata.               ===\n");
    }
    else
    {
        for (int i = 0; i < totalEquipUrgentes; i++)
        {
            printf("===    - [%s] %-20.20s ate %s                     ===\n",
                   equipUrgentes[i]->id,
                   equipUrgentes[i]->nome,
                   equipUrgentes[i]->proxima_manutencao);
        }
    }
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
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

    if (total_alunos > 0)
    {
        ticketMedio = receitaTotal / (double)total_alunos;
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
