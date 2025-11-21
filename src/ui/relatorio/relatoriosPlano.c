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

static void relatorioListagemPlanos(void);
static void relatorioRankingPopularidade(void);
static void relatorioReceitaPorPlano(void);
static void relatorioDistribuicaoAtividades(void);
static void relatorioOcupacaoHorarioPlano(void);

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

void moduloRelatoriosPlano(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===                 RELATORIOS DE PLANOS - MENU               ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  LISTAGEM COMPLETA DE PLANOS                         ===\n");
        printf("===  [2]  RANKING DE POPULARIDADE                             ===\n");
        printf("===  [3]  ANALISE DE RECEITA POR PLANO                        ===\n");
        printf("===  [4]  DISTRIBUICAO DE ATIVIDADES                          ===\n");
        printf("===  [5]  OCUPACAO POR HORARIO DE PLANO                       ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

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
    limparTela();

    printf("=========================================================================\n");
    printf("===                RELATORIO - LISTAGEM COMPLETA DE PLANOS           ===\n");
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
        printf("\nNenhum plano encontrado com os filtros informados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    ordenarPlanos(views, totalFiltrados, ordenacao);

    printf("\nStatus: %s | Ordenacao: ",
           filtroStatus == '1' ? "Somente ativos" : filtroStatus == '2' ? "Somente inativos"
                                                                        : "Todos");
    switch (ordenacao)
    {
    case '2':
        printf("Alunos vinculados");
        break;
    case '3':
        printf("Valor do plano");
        break;
    case '1':
    default:
        printf("Nome");
        break;
    }
    printf(" | Termo: %s\n", termo[0] != '\0' ? termo : "Nenhum");
    printf("-------------------------------------------------------------------------\n");

    double receitaTotal = 0.0;
    int alunosTotais = 0;

    for (int i = 0; i < totalFiltrados; i++)
    {
        const struct plano *plano = views[i].plano;
        printf("[%s] %s\n", plano->id, plano->nome);
        printf("Horario: %s - %s | Valor: R$ %.2f | Status: %s\n",
               plano->horario_inicio,
               plano->horario_fim,
               plano->valor,
               plano->ativo ? "Ativo" : "Inativo");

        if (plano->total_atividades > 0)
        {
            printf("Atividades:\n");
            for (int j = 0; j < plano->total_atividades; j++)
            {
                printf("  - %s\n", plano->atividades[j]);
            }
        }
        else
        {
            printf("Atividades: Nenhuma cadastrada.\n");
        }

        printf("Alunos vinculados: %d | Receita estimada: R$ %.2f\n",
               views[i].alunosVinculados,
               views[i].receita);
        printf("-------------------------------------------------------------------------\n");

        alunosTotais += views[i].alunosVinculados;
        receitaTotal += views[i].receita;
    }

    printf("Total de planos listados: %d\n", totalFiltrados);
    printf("Total de alunos vinculados: %d\n", alunosTotais);
    printf("Receita total estimada: R$ %.2f\n", receitaTotal);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioRankingPopularidade(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                  RELATORIO - RANKING DE POPULARIDADE              ===\n");
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
        printf("Nenhum aluno vinculado a planos para gerar o ranking.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
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

    for (int i = 0; i < totalRanking; i++)
    {
        double percentual = totalAlunos > 0 ? ((double)ranking[i].alunosVinculados / totalAlunos) * 100.0 : 0.0;
        printf("%dº - %s: %d aluno%s (%.2f%%)\n",
               i + 1,
               ranking[i].plano->nome,
               ranking[i].alunosVinculados,
               ranking[i].alunosVinculados == 1 ? "" : "s",
               percentual);
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioReceitaPorPlano(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                RELATORIO - ANALISE DE RECEITA POR PLANO           ===\n");
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
        printf("Nao ha planos que atendam ao filtro informado.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    const struct PlanoView *maior = &views[0];
    const struct PlanoView *menor = &views[0];

    printf("+------------------------------+----------+-------------+-------------+-------------+\n");
    printf("| Plano                        | Alunos   | Rec. Mensal | Rec. Anual  | %% Receita  |\n");
    printf("+------------------------------+----------+-------------+-------------+-------------+\n");

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

        double percentual = receitaTotal > 0.0 ? (views[i].receita / receitaTotal) * 100.0 : 0.0;

        printf("| %-28.28s | %8d | R$ %9.2f | R$ %9.2f | %9.2f |\n",
               views[i].plano->nome,
               views[i].alunosVinculados,
               views[i].receita,
               views[i].receita * 12.0,
               percentual);
    }

    printf("+------------------------------+----------+-------------+-------------+-------------+\n");
    printf("Totais: Receita mensal = R$ %.2f | Receita anual = R$ %.2f\n",
           receitaTotal,
           receitaTotal * 12.0);
    printf("Maior receita: %s (R$ %.2f)\n", maior->plano->nome, maior->receita);
    printf("Menor receita: %s (R$ %.2f)\n", menor->plano->nome, menor->receita);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioDistribuicaoAtividades(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===            RELATORIO - DISTRIBUICAO DE ATIVIDADES                ===\n");
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
            if (atividade[0] == '\0')
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
        printf("Nenhuma atividade cadastrada nos planos.\n");
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

    printf("+------------------------------+------------+-----------+\n");
    printf("| Atividade                    | Planos     | Alunos    |\n");
    printf("+------------------------------+------------+-----------+\n");
    for (int i = 0; i < totalAtividades; i++)
    {
        double percentual = totalAlunosAcessando > 0 ? ((double)atividades[i].alunos / totalAlunosAcessando) * 100.0 : 0.0;
        printf("| %-28.28s | %10d | %7d (%5.2f%%) |\n",
               atividades[i].nome,
               atividades[i].planos,
               atividades[i].alunos,
               percentual);
    }
    printf("+------------------------------+------------+-----------+\n");
    printf("Total de atividades analisadas: %d\n", totalAtividades);
    printf("Total de alunos com acesso: %d\n", totalAlunosAcessando);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void relatorioOcupacaoHorarioPlano(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===             RELATORIO - OCUPACAO POR HORARIO DE PLANO            ===\n");
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

    enum
    {
        FAIXA_MANHA = 0,
        FAIXA_TARDE,
        FAIXA_NOITE,
        FAIXA_OUTROS,
        TOTAL_FAIXAS
    };

    const char *faixasNome[TOTAL_FAIXAS] = {
        "MANHA (06:00 - 12:00)",
        "TARDE (12:00 - 18:00)",
        "NOITE (18:00 - 22:00)",
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
        if (inicio >= 6 && inicio < 12)
        {
            faixa = FAIXA_MANHA;
        }
        else if (inicio >= 12 && inicio < 18)
        {
            faixa = FAIXA_TARDE;
        }
        else if (inicio >= 18 && inicio < 22)
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
        printf("\n%s\n", faixasNome[faixa]);
        printf("----------------------------------------\n");

        if (contadores[faixa] == 0)
        {
            printf("Nenhum plano nesta faixa.\n");
        }

        for (int i = 0; i < contadores[faixa]; i++)
        {
            printf("  - %s: %d aluno%s\n",
                   distribuicao[faixa][i].plano->nome,
                   distribuicao[faixa][i].alunos,
                   distribuicao[faixa][i].alunos == 1 ? "" : "s");
        }

        printf("  Total: %d aluno%s\n",
               alunosPorFaixa[faixa],
               alunosPorFaixa[faixa] == 1 ? "" : "s");
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static char selecionarStatusPlano(void)
{
    while (1)
    {
        printf("\nSelecione o status dos planos:\n");
        printf("[1] Somente ativos\n");
        printf("[2] Somente inativos\n");
        printf("[3] Todos\n");

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
    printf("\nDigite parte do nome, ID ou atividade para buscar (ENTER para ignorar):\n");
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
        printf("\nSelecione a ordenacao desejada:\n");
        printf("[1] Nome\n");
        printf("[2] Quantidade de alunos\n");
        printf("[3] Valor do plano\n");

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
