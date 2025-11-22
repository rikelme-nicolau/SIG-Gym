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

static char criterioOrdenacaoFuncionario;

static void relatorioListagemFuncionarios(void);
static void relatorioFuncionariosPorCargo(void);
static void relatorioDistribuicaoFaixaEtaria(void);
static void relatorioAnaliseEquipe(void);
static char selecionarFiltroStatusFuncionario(void);
static void solicitarFiltroCargo(char *dest, size_t tamanho);
static char selecionarOrdenacaoFuncionario(void);
static int compararFuncionarios(const void *a, const void *b);

void moduloRelatoriosFuncionario(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===             RELATORIOS DE FUNCIONARIOS - MENU             ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  LISTAGEM COMPLETA DE FUNCIONARIOS                   ===\n");
        printf("===  [2]  FUNCIONARIOS POR CARGO                              ===\n");
        printf("===  [3]  DISTRIBUICAO POR FAIXA ETARIA                       ===\n");
        printf("===  [4]  ANALISE DE EQUIPE                                   ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

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
    limparTela();

    printf("=========================================================================\n");
    printf("===           RELATORIO - LISTAGEM COMPLETA DE FUNCIONARIOS          ===\n");
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
        printf("Nenhum funcionario atende aos filtros informados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    qsort(lista, totalLista, sizeof(const struct funcionario *), compararFuncionarios);

    const char *setaNome = (criterioOrdenacaoFuncionario == '1' || criterioOrdenacaoFuncionario == '4') ? "▲" : "";
    const char *setaCargo = (criterioOrdenacaoFuncionario == '2' || criterioOrdenacaoFuncionario == '4') ? "▲" : "";
    const char *setaIdade = (criterioOrdenacaoFuncionario == '3' || criterioOrdenacaoFuncionario == '5') ? "▲" : (criterioOrdenacaoFuncionario == '6' ? "▼" : "");
    printf(">>> Ordenando por ");
    switch (criterioOrdenacaoFuncionario)
    {
    case '2':
        printf("Cargo %s\n", setaCargo);
        break;
    case '3':
        printf("Idade %s\n", setaIdade);
        break;
    case '4':
        printf("Cargo %s com desempate por Nome %s\n", setaCargo, setaNome);
        break;
    case '5':
        printf("Idade %s (crescente)\n", setaIdade);
        break;
    case '6':
        printf("Idade %s (decrescente)\n", setaIdade);
        break;
    case '1':
    default:
        printf("Nome %s\n", setaNome);
        break;
    }

    printf("+-----------+----------------------+----------------------+-------+----------------------+\n");
    printf("| ID        | Nome %-15s | Cargo %-15s | Idade %-1s | Telefone             |\n",
           setaNome,
           setaCargo,
           setaIdade);
    printf("+-----------+----------------------+----------------------+-------+----------------------+\n");

    for (int i = 0; i < totalLista; i++)
    {
        const struct funcionario *f = lista[i];
        printf("| %-9.9s | %-20.20s | %-20.20s | %5d | %-20.20s |\n",
               f->id,
               f->nome,
               f->cargo,
               f->idade,
               f->telefone);
    }

    printf("+-----------+----------------------+----------------------+-------+----------------------+\n");
    printf("Total listado: %d\n", totalLista);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioFuncionariosPorCargo(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===              RELATORIO - FUNCIONARIOS POR CARGO                  ===\n");
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
        printf("Nenhum cargo informado para os funcionarios.\n");
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
                struct CargoView tmp = cargos[i];
                cargos[i] = cargos[j];
                cargos[j] = tmp;
            }
        }
    }

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

        printf("\n%s (%d funcionario%s)\n",
               cargos[i].nome,
               cargos[i].total,
               cargos[i].total == 1 ? "" : "s");
        printf("--------------------------------------------------------------\n");
        for (int j = 0; j < cargos[i].total; j++)
        {
            printf("  - [%s] %s (%d anos)\n",
                   cargos[i].lista[j]->id,
                   cargos[i].lista[j]->nome,
                   cargos[i].lista[j]->idade);
        }
        printf("  Subtotal: %d | Idade media: %.0f | Mais novo: %s (%d) | Mais velho: %s (%d)\n",
               cargos[i].total,
               media,
               maisNovo->nome,
               maisNovo->idade,
               maisVelho->nome,
               maisVelho->idade);
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioDistribuicaoFaixaEtaria(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===        RELATORIO - DISTRIBUICAO POR FAIXA ETARIA (FUNCIONARIOS)   ===\n");
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
        printf("Nao foi possivel calcular idades validas para os funcionarios.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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

    printf("\nDistribuicao por faixa:\n");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentual = (double)contadores[i] / totalConsiderados * 100.0;
        printf("%-12s: %3d funcionario%s (%.2f%%)\n",
               labels[i],
               contadores[i],
               contadores[i] == 1 ? "" : "s",
               percentual);
        if (contadores[i] > 0)
        {
            printf("  Nomes:\n");
            for (int j = 0; j < contadores[i]; j++)
            {
                printf("    - %s (%d anos)\n",
                       faixas[i][j]->nome,
                       faixas[i][j]->idade);
            }
        }
    }

    printf("\nFaixa predominante: %s (%.2f%%)\n", labels[faixaPredominante], percentualPredominante);
    printf("Idade media da equipe: %.1f anos\n", idadeMedia);
    if (desequilibrio)
    {
        printf("Aviso: ha concentracao significativa em %s. Avalie diversidade etaria.\n", labels[faixaPredominante]);
    }
    else
    {
        printf("Distribuicao etaria equilibrada.\n");
    }
    printf("Total considerado: %d funcionario%s\n", totalConsiderados, totalConsiderados == 1 ? "" : "s");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioAnaliseEquipe(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                    RELATORIO - ANALISE DE EQUIPE                 ===\n");
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

    printf("Funcionarios ativos: %d | Inativos: %d\n", ativos, inativos);
    printf("Distribuicao por cargo:\n");

    for (int i = 0; i < totalCargos; i++)
    {
        double percentual = (double)cargos[i].total / total_funcionarios * 100.0;
        double media = cargos[i].total > 0 ? (double)cargos[i].somaIdade / cargos[i].total : 0.0;
        printf("  - %s: %d funcionario%s (%.2f%%) | Idade media: %.1f\n",
               cargos[i].nome,
               cargos[i].total,
               cargos[i].total == 1 ? "" : "s",
               percentual,
               media);
    }

    double proporcao = ativos > 0 ? (double)total_alunos / ativos : 0.0;
    printf("\nProporcao alunos/funcionario: 1 : %.2f\n", proporcao);
    if (proporcao > 30.0)
    {
        printf(">>> Alerta: proporcao elevada. Avalie contratar mais funcionarios.\n");
    }

    for (int i = 0; i < totalCargos; i++)
    {
        if (cargos[i].total <= 1)
        {
            printf(">>> Atenção: cargo '%s' com apenas %d profissional.\n", cargos[i].nome, cargos[i].total);
        }
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static char selecionarFiltroStatusFuncionario(void)
{
    printf("Filtro de status:\n");
    printf("[1] Somente ativos\n");
    printf("[2] Somente inativos\n");
    printf("[3] Todos\n");

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
    printf("Filtrar por cargo? (ENTER para todos): ");
    if (fgets(dest, tamanho, stdin) != NULL)
    {
        dest[strcspn(dest, "\n")] = '\0';
    }
}

static char selecionarOrdenacaoFuncionario(void)
{
    printf("Ordenacao:\n");
    printf("[1] Nome\n");
    printf("[2] Cargo\n");
    printf("[3] Idade\n");
    printf("[4] Cargo + Nome\n");
    printf("[5] Idade (crescente)\n");
    printf("[6] Idade (decrescente)\n");

    char op = lerTecla();
    if (op >= '1' && op <= '6')
    {
        return op;
    }

    opInvalida();
    return selecionarOrdenacaoFuncionario();
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
