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

static void relatorioTodasPessoasUnificado(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===              RELATORIO - TODAS AS PESSOAS (ALUNOS+FUNC)          ===\n");
    printf("=========================================================================\n");

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
        printf("Nao ha alunos ou funcionarios ativos para listar.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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
        printf("Nao foi possivel unificar as listas.\n");
        free(pessoas);
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    int contAlunos = 0;
    int contFuncs = 0;

    printf("+------+---------+----------------------+-------+\n");
    printf("| Tipo | ID      | Nome                 | Idade |\n");
    printf("+------+---------+----------------------+-------+\n");
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
        printf("|  %c   | %-7.7s | %-20.20s | %5d |\n",
               p->tipo,
               p->id,
               p->nome,
               p->idade);
    }
    printf("+------+---------+----------------------+-------+\n");
    printf("Total alunos: %d | Total funcionarios: %d | Total geral: %d\n", contAlunos, contFuncs, totalPessoas);

    printf("\nDeseja exportar para CSV? (S/N): ");
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

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
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
    printf("Arquivo CSV gerado: relatorio_pessoas_unificado.csv\n");
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
    limparTela();

    printf("=========================================================================\n");
    printf("===                  RELAÇÃO ALUNO / PLANO                            ===\n");
    printf("=========================================================================\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("| ID       | NOME DO ALUNO                | PLANO                | VALOR (R$) |\n");
    printf("+----------+------------------------------+----------------------+------------+\n");

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

        const char *nome_plano = "SEM PLANO";
        double valor_plano = 0.0;

        if (plano != NULL)
        {
            nome_plano = plano->nome;
            valor_plano = plano->valor;
        }

        printf("| %-8.8s | %-28.28s | %-20.20s | %10.2f |\n",
               lista_alunos[i].id[0] != '\0' ? lista_alunos[i].id : "-",
               lista_alunos[i].nome,
               nome_plano,
               valor_plano);
        total_relacoes++;
    }

    if (total_relacoes == 0)
    {
        printf("Nenhuma relação aluno/plano encontrada.\n");
    }

    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("Total de registros: %d\n", total_relacoes);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioCruzadoPlanoHorarioAlunos(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===           RELATORIO - PLANO x HORARIO x ALUNOS (CRUZADO)         ===\n");
    printf("=========================================================================\n");

    if (total_planos == 0)
    {
        printf("Nao ha planos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    struct PlanoHorarioView
    {
        const struct plano *plano;
        int faixa;
        int alunos;
    };

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
        printf("Nao ha dados suficientes para gerar este relatorio.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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

    printf("+----------+------------------------------+------------+--------+-------------+\n");
    printf("| HORARIO  | PLANO                        | ALUNOS     | %% HORA | %% TOTAL     |\n");
    printf("+----------+------------------------------+------------+--------+-------------+\n");

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
        printf("| %-8s | %-28.28s | %10d | %6.1f | %9.1f |\n",
               descricaoFaixa(views[i].faixa),
               views[i].plano->nome,
               views[i].alunos,
               percHora,
               percTotal);
    }

    printf("+----------+------------------------------+------------+--------+-------------+\n");
    if (faixaMaior != -1)
    {
        printf("Faixa com maior carga: %s (%.1f%% do total)\n", descricaoFaixa(faixaMaior), maiorCarga);
    }
    if (faixaMenor != -1)
    {
        printf("Faixa com menor carga: %s (%.1f%% do total)\n", descricaoFaixa(faixaMenor), menorCarga);
    }
    if (maiorCarga > 60.0)
    {
        printf(">>> Atencao: faixa %s pode estar sobrecarregada.\n", descricaoFaixa(faixaMaior));
    }
    if (menorCarga < 15.0)
    {
        printf(">>> Observacao: faixa %s apresenta baixa ocupacao.\n", descricaoFaixa(faixaMenor));
    }

    printf("Total de alunos considerados: %d\n", alunosTotais);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioEquipamentoCategoriaManutencao(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===      RELATORIO - EQUIPAMENTO x CATEGORIA x MANUTENCAO (CRUZADO)   ===\n");
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

    struct CategoriaEquip categorias[MAX_EQUIPAMENTOS];
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
        printf("\nCATEGORIA: %s\n", categorias[i].nome);
        printf("+------------+----------------------+-------------+-------------+-----------+\n");
        printf("| ID         | EQUIPAMENTO          | ULTIMA      | PROXIMA     | STATUS    |\n");
        printf("+------------+----------------------+-------------+-------------+-----------+\n");

        for (int j = 0; j < categorias[i].total; j++)
        {
            const struct equipamento *eq = categorias[i].lista[j];
            double dias = categorias[i].dias[j];
            const char *status = "✓ EM DIA";
            if (dias < 0)
            {
                status = "✗ VENCIDA";
            }
            else if (dias <= 7)
            {
                status = "⚠ PROXIMA";
            }

            printf("| %-10s | %-20.20s | %-11s | %-11s | %-9s |\n",
                   eq->id,
                   eq->nome,
                   eq->ultima_manutencao,
                   eq->proxima_manutencao,
                   status);
        }

        printf("+------------+----------------------+-------------+-------------+-----------+\n");
        printf("Subtotal: %d equipamento%s | %d em dia, %d proximas, %d vencidas\n",
               categorias[i].total,
               categorias[i].total == 1 ? "" : "s",
               categorias[i].emDia,
               categorias[i].proximas,
               categorias[i].vencidas);
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioCruzadoFuncionarioCargoIdade(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===        RELATORIO - FUNCIONARIO x CARGO x FAIXA ETARIA (CRUZADO)   ===\n");
    printf("=========================================================================\n");

    if (total_funcionarios == 0)
    {
        printf("Nao ha funcionarios cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    enum
    {
        FAIXA_18_25 = 0,
        FAIXA_26_35,
        FAIXA_36_45,
        FAIXA_46_55,
        FAIXA_56_MAIS,
        TOTAL_FAIXAS_FUNC
    };

    const char *labels[TOTAL_FAIXAS_FUNC] = {
        "18-25",
        "26-35",
        "36-45",
        "46-55",
        "56+"};

    struct CargoFaixa
    {
        char nome[64];
        int total;
        int faixas[TOTAL_FAIXAS_FUNC];
        int somaIdade;
    };

    struct CargoFaixa cargos[MAX_FUNCIONARIOS];
    int totalCargos = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].cargo[0] == '\0' || lista_funcionarios[i].idade <= 0)
        {
            continue;
        }

        int idx = -1;
        for (int c = 0; c < totalCargos; c++)
        {
            if (strcasecmp(cargos[c].nome, lista_funcionarios[i].cargo) == 0)
            {
                idx = c;
                break;
            }
        }

        if (idx == -1 && totalCargos < MAX_FUNCIONARIOS)
        {
            strncpy(cargos[totalCargos].nome, lista_funcionarios[i].cargo, sizeof(cargos[totalCargos].nome));
            cargos[totalCargos].nome[sizeof(cargos[totalCargos].nome) - 1] = '\0';
            cargos[totalCargos].total = 0;
            cargos[totalCargos].somaIdade = 0;
            memset(cargos[totalCargos].faixas, 0, sizeof(cargos[totalCargos].faixas));
            idx = totalCargos++;
        }

        if (idx == -1)
        {
            continue;
        }

        int idade = lista_funcionarios[i].idade;
        int faixaIndex = FAIXA_56_MAIS;
        if (idade >= 18 && idade <= 25)
        {
            faixaIndex = FAIXA_18_25;
        }
        else if (idade <= 35)
        {
            faixaIndex = FAIXA_26_35;
        }
        else if (idade <= 45)
        {
            faixaIndex = FAIXA_36_45;
        }
        else if (idade <= 55)
        {
            faixaIndex = FAIXA_46_55;
        }

        cargos[idx].total++;
        cargos[idx].faixas[faixaIndex]++;
        cargos[idx].somaIdade += idade;
    }

    if (totalCargos == 0)
    {
        printf("Nao ha cargos suficientes para analise.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    for (int i = 0; i < totalCargos - 1; i++)
    {
        for (int j = i + 1; j < totalCargos; j++)
        {
            if (strcasecmp(cargos[j].nome, cargos[i].nome) < 0)
            {
                struct CargoFaixa tmp = cargos[i];
                cargos[i] = cargos[j];
                cargos[j] = tmp;
            }
        }
    }

    printf("+---------------+-------+----------+----------+----------+----------+-------------+\n");
    printf("| CARGO         | TOTAL | 18-25    | 26-35    | 36-45    | 46-55    | 56+ | MEDIA |\n");
    printf("+---------------+-------+----------+----------+----------+----------+-------------+\n");

    double menorMedia = 1000.0;
    double maiorMedia = 0.0;
    const char *cargoMaisJovem = NULL;
    const char *cargoMaisExperiente = NULL;

    for (int i = 0; i < totalCargos; i++)
    {
        double mediaIdade = cargos[i].total > 0 ? (double)cargos[i].somaIdade / cargos[i].total : 0.0;

        if (mediaIdade < menorMedia)
        {
            menorMedia = mediaIdade;
            cargoMaisJovem = cargos[i].nome;
        }
        if (mediaIdade > maiorMedia)
        {
            maiorMedia = mediaIdade;
            cargoMaisExperiente = cargos[i].nome;
        }

        printf("| %-13.13s | %5d |",
               cargos[i].nome,
               cargos[i].total);

        for (int faixa = 0; faixa < TOTAL_FAIXAS_FUNC; faixa++)
        {
            double perc = cargos[i].total > 0 ? ((double)cargos[i].faixas[faixa] / cargos[i].total) * 100.0 : 0.0;
            printf(" %2d (%3.0f%%) |",
                   cargos[i].faixas[faixa],
                   perc);
        }

        printf(" %10.1f |\n", mediaIdade);
    }

    printf("+---------------+-------+----------+----------+----------+----------+-------------+\n");

    printf("Cargo mais jovem: %s (media %.1f anos)\n", cargoMaisJovem ? cargoMaisJovem : "-", menorMedia);
    printf("Cargo mais experiente: %s (media %.1f anos)\n", cargoMaisExperiente ? cargoMaisExperiente : "-", maiorMedia);

    if (cargoMaisJovem && cargoMaisExperiente)
    {
        printf("Observacao: %s possui equipe mais jovem em relacao a %s.\n",
               cargoMaisJovem,
               cargoMaisExperiente);
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioAlunoIdadePlano(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===          RELATORIO - ALUNO x IDADE x PLANO (CRUZADO)             ===\n");
    printf("=========================================================================\n");

    if (total_alunos == 0)
    {
        printf("Nao ha alunos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    enum
    {
        F18_25 = 0,
        F26_35,
        F36_45,
        F46_55,
        F56_MAIS,
        TOTAL_FAIXAS_ALUNO
    };

    const char *labels[TOTAL_FAIXAS_ALUNO] = {
        "18-25 anos",
        "26-35 anos",
        "36-45 anos",
        "46-55 anos",
        "56+ anos"};

    struct PlanoCount
    {
        const struct plano *plano;
        int total;
        bool semPlano;
    };

    struct FaixaAnalise
    {
        struct PlanoCount planos[MAX_PLANOS + 1];
        int totalPlanos;
        int totalAlunos;
    };

    struct FaixaAnalise faixas[TOTAL_FAIXAS_ALUNO];
    memset(faixas, 0, sizeof(faixas));

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        int idade = atoi(lista_alunos[i].idade);
        if (idade <= 0)
        {
            continue;
        }

        int faixaIdx = F56_MAIS;
        if (idade >= 18 && idade <= 25)
        {
            faixaIdx = F18_25;
        }
        else if (idade <= 35)
        {
            faixaIdx = F26_35;
        }
        else if (idade <= 45)
        {
            faixaIdx = F36_45;
        }
        else if (idade <= 55)
        {
            faixaIdx = F46_55;
        }

        const struct plano *plano = buscarPlanoPorId(lista_alunos[i].plano_id);
        bool semPlano = (plano == NULL);

        int idx = -1;
        for (int p = 0; p < faixas[faixaIdx].totalPlanos; p++)
        {
            if (faixas[faixaIdx].planos[p].semPlano == semPlano &&
                ((semPlano && plano == NULL) ||
                 (!semPlano && faixas[faixaIdx].planos[p].plano == plano)))
            {
                idx = p;
                break;
            }
        }

        if (idx == -1 && faixas[faixaIdx].totalPlanos < (int)(sizeof(faixas[faixaIdx].planos) / sizeof(faixas[faixaIdx].planos[0])))
        {
            idx = faixas[faixaIdx].totalPlanos++;
            faixas[faixaIdx].planos[idx].plano = plano;
            faixas[faixaIdx].planos[idx].total = 0;
            faixas[faixaIdx].planos[idx].semPlano = semPlano;
        }

        if (idx != -1)
        {
            faixas[faixaIdx].planos[idx].total++;
            faixas[faixaIdx].totalAlunos++;
        }
    }

    for (int faixa = 0; faixa < TOTAL_FAIXAS_ALUNO; faixa++)
    {
        if (faixas[faixa].totalAlunos == 0)
        {
            continue;
        }

        for (int i = 0; i < faixas[faixa].totalPlanos - 1; i++)
        {
            for (int j = i + 1; j < faixas[faixa].totalPlanos; j++)
            {
                if (faixas[faixa].planos[j].total > faixas[faixa].planos[i].total)
                {
                    struct PlanoCount tmp = faixas[faixa].planos[i];
                    faixas[faixa].planos[i] = faixas[faixa].planos[j];
                    faixas[faixa].planos[j] = tmp;
                }
            }
        }

        printf("\nFAIXA ETARIA: %s (%d aluno%s)\n",
               labels[faixa],
               faixas[faixa].totalAlunos,
               faixas[faixa].totalAlunos == 1 ? "" : "s");

        const char *dominante = NULL;
        double maiorPerc = 0.0;

        for (int i = 0; i < faixas[faixa].totalPlanos; i++)
        {
            const struct PlanoCount *pc = &faixas[faixa].planos[i];
            double perc = (double)pc->total / faixas[faixa].totalAlunos * 100.0;
            const char *nomePlano = pc->semPlano ? "Sem plano" : pc->plano->nome;
            printf("  - %s: %d aluno%s (%.1f%%)\n",
                   nomePlano,
                   pc->total,
                   pc->total == 1 ? "" : "s",
                   perc);

            if (perc > maiorPerc)
            {
                maiorPerc = perc;
                dominante = nomePlano;
            }
        }

        if (dominante != NULL)
        {
            printf("  Mais popular: %s (%.1f%%)\n", dominante, maiorPerc);
            if (maiorPerc > 60.0)
            {
                printf("  >> Observacao: alta concentracao em %s nesta faixa.\n", dominante);
            }
        }
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioPlanoAtividadePopularidade(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===        RELATORIO - PLANO x ATIVIDADE x POPULARIDADE (CRUZADO)    ===\n");
    printf("=========================================================================\n");

    if (total_planos == 0)
    {
        printf("Nao ha planos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    struct AtividadeRanking
    {
        char nome[64];
        const struct plano *planos[MAX_PLANOS];
        int totalPlanos;
        int alunos;
    };

    struct AtividadeRanking atividades[MAX_PLANOS * MAX_ATIVIDADES];
    int totalAtividades = 0;
    int alunosTotais = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunosTotais++;
        }
    }

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int alunosPlano = contarAlunosDoPlano(lista_planos[i].id);
        for (int j = 0; j < lista_planos[i].total_atividades; j++)
        {
            const char *atividade = lista_planos[i].atividades[j];
            if (atividade[0] == '\0')
            {
                continue;
            }

            char nomeAtv[64];
            size_t len = strlen(atividade);
            if (len >= sizeof(nomeAtv))
            {
                len = sizeof(nomeAtv) - 1;
            }
            for (size_t c = 0; c < len; c++)
            {
                nomeAtv[c] = (char)toupper((unsigned char)atividade[c]);
            }
            nomeAtv[len] = '\0';

            int idx = -1;
            for (int a = 0; a < totalAtividades; a++)
            {
                if (strcmp(atividades[a].nome, nomeAtv) == 0)
                {
                    idx = a;
                    break;
                }
            }

            if (idx == -1 && totalAtividades < (int)(sizeof(atividades) / sizeof(atividades[0])))
            {
                strncpy(atividades[totalAtividades].nome, nomeAtv, sizeof(atividades[totalAtividades].nome));
                atividades[totalAtividades].nome[sizeof(atividades[totalAtividades].nome) - 1] = '\0';
                atividades[totalAtividades].totalPlanos = 0;
                atividades[totalAtividades].alunos = 0;
                idx = totalAtividades++;
            }

            if (idx != -1)
            {
                bool jaContado = false;
                for (int p = 0; p < atividades[idx].totalPlanos; p++)
                {
                    if (atividades[idx].planos[p] == &lista_planos[i])
                    {
                        jaContado = true;
                        break;
                    }
                }

                if (!jaContado && atividades[idx].totalPlanos < MAX_PLANOS)
                {
                    atividades[idx].planos[atividades[idx].totalPlanos++] = &lista_planos[i];
                }
                atividades[idx].alunos += alunosPlano;
            }
        }
    }

    if (totalAtividades == 0 || alunosTotais == 0)
    {
        printf("Nao ha dados suficientes para gerar o ranking.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    for (int i = 0; i < totalAtividades - 1; i++)
    {
        for (int j = i + 1; j < totalAtividades; j++)
        {
            if (atividades[j].alunos > atividades[i].alunos)
            {
                struct AtividadeRanking tmp = atividades[i];
                atividades[i] = atividades[j];
                atividades[j] = tmp;
            }
        }
    }

    printf("+----------------------+----------+---------------------------+-------------+\n");
    printf("| ATIVIDADE            | ALUNOS   | PLANOS QUE OFERECEM       | %% ALCANCE   |\n");
    printf("+----------------------+----------+---------------------------+-------------+\n");

    for (int i = 0; i < totalAtividades; i++)
    {
        double perc = (double)atividades[i].alunos / alunosTotais * 100.0;

        char planosBuf[128] = "";
        for (int p = 0; p < atividades[i].totalPlanos; p++)
        {
            if (p > 0)
            {
                strncat(planosBuf, ", ", sizeof(planosBuf) - strlen(planosBuf) - 1);
            }
            strncat(planosBuf, atividades[i].planos[p]->nome, sizeof(planosBuf) - strlen(planosBuf) - 1);
        }

        printf("| %-20.20s | %8d | %-25.25s | %11.2f |\n",
               atividades[i].nome,
               atividades[i].alunos,
               planosBuf[0] != '\0' ? planosBuf : "Nenhum",
               perc);
    }

    printf("+----------------------+----------+---------------------------+-------------+\n");
    printf("Total de atividades analisadas: %d | Alunos considerados: %d\n", totalAtividades, alunosTotais);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
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
