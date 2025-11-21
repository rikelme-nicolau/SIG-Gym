#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosAluno.h"
#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "ui/utils/lerTecla.h"
#include "src/ui/utils/validarNascimento.h"

void relatorioListagemCompleta(void);
void relatorioAlunosPorPlano(void);
void relatorioAlunosPorFaixaEtaria(void);
void relatorioAlunosPorRegiao(void);
void relatorioAlunosAtivosVsInativos(void);
void relatorioAlunosSemPlano(void);

struct aluno_view
{
    const struct aluno *aluno;
    const struct plano *plano;
};

static char selecionarFiltroStatus(void);
static void solicitarTermoBusca(char *dest, size_t tamanho);
static char selecionarOrdenacao(void);
static bool alunoCorrespondeFiltros(const struct aluno *aluno, const struct plano *plano, char filtroStatus, const char *termo);
static bool statusCorresponde(bool ativo, char filtroStatus);
static bool textoContemInsensitive(const char *texto, const char *busca);
static void ordenarResultados(struct aluno_view *lista, int total, char criterio);
static int compararViews(const struct aluno_view *a, const struct aluno_view *b, char criterio);
static int compararStringsCaseInsensitive(const char *a, const char *b);
static const struct plano *buscarPlanoPorIdLocal(const char *id);
static const char *descricaoStatusAluno(char status);
static const char *descricaoOrdenacao(char criterio);
static void exportarAlunosCsv(const struct aluno_view *lista, int total);
static void extrairRegiao(const char *endereco, char *dest, size_t tamanho);

void moduloRelatoriosAluno(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===                 RELATORIOS DE ALUNOS - MENU               ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  LISTAGEM COMPLETA (COM FILTROS)                     ===\n");
        printf("===  [2]  ALUNOS POR PLANO                                    ===\n");
        printf("===  [3]  ALUNOS POR FAIXA ETARIA                             ===\n");
        printf("===  [4]  ALUNOS POR REGIAO                                   ===\n");
        printf("===  [5]  ALUNOS ATIVOS VS INATIVOS                           ===\n");
        printf("===  [6]  ALUNOS SEM PLANO DEFINIDO                           ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

        op = lerTecla();
        limparTela();

        switch (op)
        {
        case '1':
            relatorioListagemCompleta();
            break;

        case '2':
            relatorioAlunosPorPlano();
            break;

        case '3':
            relatorioAlunosPorFaixaEtaria();
            break;

        case '4':
            relatorioAlunosPorRegiao();
            break;

        case '5':
            relatorioAlunosAtivosVsInativos();
            break;

        case '6':
            relatorioAlunosSemPlano();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

void relatorioListagemCompleta(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===            RELATORIO - LISTAGEM COMPLETA DE ALUNOS               ===\n");
    printf("=========================================================================\n");

    char filtroStatus = selecionarFiltroStatus();
    char ordenacao = selecionarOrdenacao();

    char termo[256];
    solicitarTermoBusca(termo, sizeof(termo));

    struct aluno_view resultados[MAX_ALUNOS];
    int totalFiltrados = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        const struct plano *plano = buscarPlanoPorIdLocal(lista_alunos[i].plano_id);
        if (!alunoCorrespondeFiltros(&lista_alunos[i], plano, filtroStatus, termo))
        {
            continue;
        }

        resultados[totalFiltrados].aluno = &lista_alunos[i];
        resultados[totalFiltrados].plano = plano;
        totalFiltrados++;
    }

    if (totalFiltrados == 0)
    {
        printf("\nNenhum aluno encontrado com os filtros informados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER> para voltar...");
        getchar();
        limparTela();
        return;
    }

    ordenarResultados(resultados, totalFiltrados, ordenacao);

    printf("\nStatus: %s | Ordenacao: %s | Termo: %s\n",
           descricaoStatusAluno(filtroStatus),
           descricaoOrdenacao(ordenacao),
           termo[0] != '\0' ? termo : "Nenhum");
    printf("-------------------------------------------------------------------------\n");

    const int porPagina = 10;
    int totalPaginas = (totalFiltrados + porPagina - 1) / porPagina;

    for (int pagina = 0; pagina < totalPaginas; pagina++)
    {
        int inicio = pagina * porPagina;
        int fim = inicio + porPagina;
        if (fim > totalFiltrados)
        {
            fim = totalFiltrados;
        }

        printf("\n--- Pagina %d de %d ----------------------------------------------\n", pagina + 1, totalPaginas);

        for (int i = inicio; i < fim; i++)
        {
            const struct aluno *aluno = resultados[i].aluno;
            const char *planoNome = resultados[i].plano != NULL ? resultados[i].plano->nome : "Sem plano";
            const char *planoId = (aluno->plano_id[0] != '\0') ? aluno->plano_id : "N/A";

            printf("[%s] %s\n", aluno->id, aluno->nome);
            printf("    Plano: %s (%s) | Status: %s\n",
                   planoNome,
                   planoId,
                   aluno->ativo ? "Ativo" : "Inativo");
        }

        if (pagina < totalPaginas - 1)
        {
            printf("\n>>> Pressione <ENTER> para ver a proxima página...");
            getchar();
        }
    }

    printf("\n-------------------------------------------------------------------------\n");
    printf("Total de alunos listados: %d\n", totalFiltrados);

    printf("Deseja exportar o resultado para CSV? (S/N): ");
    int resposta = getchar();
    while (resposta == '\n')
    {
        resposta = getchar();
    }
    if (resposta == 's' || resposta == 'S')
    {
        exportarAlunosCsv(resultados, totalFiltrados);
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        // limpar buffer
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER> para voltar...");
    getchar();
    limparTela();
}

void relatorioAlunosPorPlano(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                 RELATORIO - ALUNOS POR PLANO                     ===\n");
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

    double receitaTotal = 0.0;
    int totalAlunos = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        int alunosPlano = 0;
        printf("\nPlano: %s (R$ %.2f) - %s às %s\n",
               lista_planos[i].nome,
               lista_planos[i].valor,
               lista_planos[i].horario_inicio,
               lista_planos[i].horario_fim);

        for (int j = 0; j < total_alunos; j++)
        {
            if (!lista_alunos[j].ativo)
            {
                continue;
            }

            if (strcmp(lista_alunos[j].plano_id, lista_planos[i].id) == 0)
            {
                printf("  - [%s] %s\n", lista_alunos[j].id, lista_alunos[j].nome);
                alunosPlano++;
            }
        }

        double receitaPlano = alunosPlano * lista_planos[i].valor;
        receitaTotal += receitaPlano;
        totalAlunos += alunosPlano;

        printf("  Subtotal: %d aluno(s) | Receita: R$ %.2f\n", alunosPlano, receitaPlano);
    }

    printf("\n=========================================================================\n");
    printf("TOTAL GERAL: %d aluno(s) | Receita total estimada: R$ %.2f\n", totalAlunos, receitaTotal);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAlunosPorFaixaEtaria(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===             RELATORIO - ALUNOS POR FAIXA ETARIA                  ===\n");
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

    int contadores[TOTAL_FAIXAS] = {0};
    const char *nomesFaixa[TOTAL_FAIXAS][MAX_ALUNOS];

    bool listarNomes = false;

    printf("Deseja listar os nomes em cada faixa? (S/N): ");
    int resposta = getchar();
    if (resposta == 's' || resposta == 'S')
    {
        listarNomes = true;
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        // limpa buffer
    }

    int alunosValidos = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        int idade = calcularIdade(lista_alunos[i].idade);
        if (idade <= 0)
        {
            continue;
        }

        int indiceFaixa;
        if (idade >= 18 && idade <= 25)
        {
            indiceFaixa = FAIXA_18_25;
        }
        else if (idade <= 35)
        {
            indiceFaixa = FAIXA_26_35;
        }
        else if (idade <= 45)
        {
            indiceFaixa = FAIXA_36_45;
        }
        else if (idade <= 55)
        {
            indiceFaixa = FAIXA_46_55;
        }
        else
        {
            indiceFaixa = FAIXA_56_MAIS;
        }

        if (listarNomes && contadores[indiceFaixa] < MAX_ALUNOS)
        {
            nomesFaixa[indiceFaixa][contadores[indiceFaixa]] = lista_alunos[i].nome;
        }

        contadores[indiceFaixa]++;
        alunosValidos++;
    }

    if (alunosValidos == 0)
    {
        printf("Nao foi possivel calcular idades validas para os alunos.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    printf("\nDistribuicao por faixa:\n");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentual = (double)contadores[i] / alunosValidos * 100.0;
        int barras = (int)(percentual / 5); // barra maxima 20 caracteres
        printf("%-10s: ", labels[i]);
        for (int b = 0; b < barras; b++)
        {
            printf("█");
        }
        for (int b = barras; b < 20; b++)
        {
            printf("░");
        }
        printf(" (%d aluno%s - %.2f%%)\n",
               contadores[i],
               contadores[i] == 1 ? "" : "s",
               percentual);

        if (listarNomes && contadores[i] > 0)
        {
            printf("  Nomes:\n");
            for (int j = 0; j < contadores[i]; j++)
            {
                printf("    - %s\n", nomesFaixa[i][j]);
            }
        }
    }

    printf("\nTotal de alunos considerados: %d\n", alunosValidos);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAlunosPorRegiao(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                 RELATORIO - ALUNOS POR REGIAO                     ===\n");
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

    struct Regiao
    {
        char nome[128];
        int quantidade;
    };

    struct Regiao regioes[MAX_ALUNOS];
    int totalRegioes = 0;
    int totalConsiderados = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        char regiao[128];
        extrairRegiao(lista_alunos[i].endereco, regiao, sizeof(regiao));

        int idx = -1;
        for (int r = 0; r < totalRegioes; r++)
        {
            if (strcmp(regioes[r].nome, regiao) == 0)
            {
                idx = r;
                break;
            }
        }
        if (idx == -1 && totalRegioes < MAX_ALUNOS)
        {
            strncpy(regioes[totalRegioes].nome, regiao, sizeof(regioes[totalRegioes].nome));
            regioes[totalRegioes].nome[sizeof(regioes[totalRegioes].nome) - 1] = '\0';
            regioes[totalRegioes].quantidade = 0;
            idx = totalRegioes;
            totalRegioes++;
        }

        if (idx != -1)
        {
            regioes[idx].quantidade++;
            totalConsiderados++;
        }
    }

    if (totalConsiderados == 0)
    {
        printf("Nao foi possivel identificar regioes nos enderecos cadastrados.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    for (int i = 0; i < totalRegioes - 1; i++)
    {
        for (int j = i + 1; j < totalRegioes; j++)
        {
            if (regioes[j].quantidade > regioes[i].quantidade ||
                (regioes[j].quantidade == regioes[i].quantidade &&
                 strcmp(regioes[j].nome, regioes[i].nome) < 0))
            {
                struct Regiao tmp = regioes[i];
                regioes[i] = regioes[j];
                regioes[j] = tmp;
            }
        }
    }

    printf("\n+------------------------------+------------+-------------+\n");
    printf("| %-28s | %-10s | %-9s |\n", "Regiao", "Quantidade", "%");
    printf("+------------------------------+------------+-------------+\n");
    for (int i = 0; i < totalRegioes; i++)
    {
        double percentual = (double)regioes[i].quantidade / totalConsiderados * 100.0;
        printf("| %-28.28s | %10d | %9.2f |\n",
               regioes[i].nome,
               regioes[i].quantidade,
               percentual);
    }
    printf("+------------------------------+------------+-------------+\n");
    printf("Total de alunos considerados: %d\n", totalConsiderados);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAlunosAtivosVsInativos(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===             RELATORIO - ALUNOS ATIVOS VS INATIVOS                ===\n");
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

    const struct aluno *ativos[MAX_ALUNOS];
    const struct aluno *inativos[MAX_ALUNOS];
    int totalAtivos = 0;
    int totalInativos = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            ativos[totalAtivos++] = &lista_alunos[i];
        }
        else
        {
            inativos[totalInativos++] = &lista_alunos[i];
        }
    }

    double taxaRetencao = total_alunos > 0 ? ((double)totalAtivos / total_alunos) * 100.0 : 0.0;
    double taxaCancelamento = total_alunos > 0 ? ((double)totalInativos / total_alunos) * 100.0 : 0.0;

    printf("ALUNOS ATIVOS (%d)                |  ALUNOS INATIVOS (%d)\n", totalAtivos, totalInativos);
    printf("---------------------------------|----------------------------------------\n");

    int maxLinhas = totalAtivos > totalInativos ? totalAtivos : totalInativos;

    for (int i = 0; i < maxLinhas; i++)
    {
        char colunaEsquerda[80] = "";
        char colunaDireita[80] = "";

        if (i < totalAtivos)
        {
            const struct aluno *alAtivo = ativos[i];
            snprintf(colunaEsquerda, sizeof(colunaEsquerda), "[%-10.10s] %-20.20s | Plano: %-15.15s",
                     alAtivo->id,
                     alAtivo->nome,
                     alAtivo->plano_id[0] != '\0' ? alAtivo->plano_id : "Nao informado");
        }

        if (i < totalInativos)
        {
            const struct aluno *alInativo = inativos[i];
            snprintf(colunaDireita, sizeof(colunaDireita), "[%-10.10s] %-20.20s | Ultimo plano: %-15.15s",
                     alInativo->id,
                     alInativo->nome,
                     alInativo->plano_id[0] != '\0' ? alInativo->plano_id : "Nao informado");
        }

        printf("%-33.33s | %-40.40s\n", colunaEsquerda, colunaDireita);
    }

    printf("\nTotal ativos : %d (%.2f%%)\n", totalAtivos, taxaRetencao);
    printf("Total inativos : %d (%.2f%%)\n", totalInativos, taxaCancelamento);
    printf("Taxa de retencao: %.2f%%\n", taxaRetencao);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAlunosSemPlano(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                RELATORIO - ALUNOS SEM PLANO DEFINIDO             ===\n");
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

    int encontrados = 0;

    printf("+---------+----------------------+----------------------+----------------------+----------+\n");
    printf("| ID      | Nome                 | Telefone             | Email                | Status   |\n");
    printf("+---------+----------------------+----------------------+----------------------+----------+\n");

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            printf("| %-7.7s | %-20.20s | %-20.20s | %-20.20s | %-8s |\n",
                   lista_alunos[i].id,
                   lista_alunos[i].nome,
                   lista_alunos[i].telefone[0] != '\0' ? lista_alunos[i].telefone : "Nao informado",
                   lista_alunos[i].email[0] != '\0' ? lista_alunos[i].email : "Nao informado",
                   lista_alunos[i].ativo ? "Ativo" : "Inativo");
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        printf("| %-78s |\n", "Todos os alunos estao vinculados a algum plano.");
    }

    printf("+---------+----------------------+----------------------+----------------------+----------+\n");
    printf("Total de alunos sem plano: %d\n", encontrados);
    printf("Sugestao: revisar o cadastro e vincular cada aluno a um plano apropriado.\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static char selecionarFiltroStatus(void)
{
    while (1)
    {
        printf("\nSelecione o status dos alunos:\n");
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

static void solicitarTermoBusca(char *dest, size_t tamanho)
{
    printf("\nDigite parte do nome, ID ou plano para buscar (ENTER para ignorar):\n");
    if (fgets(dest, tamanho, stdin) != NULL)
    {
        dest[strcspn(dest, "\n")] = '\0';
    }
    else if (tamanho > 0)
    {
        dest[0] = '\0';
    }
}

static char selecionarOrdenacao(void)
{
    while (1)
    {
        printf("\nSelecione a ordenacao desejada:\n");
        printf("[1] Nome\n");
        printf("[2] ID\n");
        printf("[3] Plano\n");

        char op = lerTecla();
        if (op == '1' || op == '2' || op == '3')
        {
            return op;
        }

        opInvalida();
    }
}

static bool alunoCorrespondeFiltros(const struct aluno *aluno, const struct plano *plano, char filtroStatus, const char *termo)
{
    if (!statusCorresponde(aluno->ativo, filtroStatus))
    {
        return false;
    }

    if (termo[0] == '\0')
    {
        return true;
    }

    if (textoContemInsensitive(aluno->nome, termo) ||
        textoContemInsensitive(aluno->id, termo) ||
        textoContemInsensitive(aluno->plano_id, termo))
    {
        return true;
    }

    if (plano != NULL && textoContemInsensitive(plano->nome, termo))
    {
        return true;
    }

    return false;
}

static bool statusCorresponde(bool ativo, char filtroStatus)
{
    switch (filtroStatus)
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

static bool textoContemInsensitive(const char *texto, const char *busca)
{
    if (texto == NULL || busca == NULL || busca[0] == '\0')
    {
        return true;
    }

    size_t lenTexto = strlen(texto);
    size_t lenBusca = strlen(busca);

    if (lenBusca == 0)
    {
        return true;
    }

    for (size_t i = 0; i < lenTexto; i++)
    {
        size_t j = 0;
        while (i + j < lenTexto &&
               j < lenBusca &&
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

static void ordenarResultados(struct aluno_view *lista, int total, char criterio)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = i + 1; j < total; j++)
        {
            if (compararViews(&lista[i], &lista[j], criterio) > 0)
            {
                struct aluno_view tmp = lista[i];
                lista[i] = lista[j];
                lista[j] = tmp;
            }
        }
    }
}

static int compararViews(const struct aluno_view *a, const struct aluno_view *b, char criterio)
{
    const char *campoA = "";
    const char *campoB = "";

    switch (criterio)
    {
    case '2':
        campoA = a->aluno->id;
        campoB = b->aluno->id;
        break;
    case '3':
        campoA = a->plano != NULL ? a->plano->nome : "";
        campoB = b->plano != NULL ? b->plano->nome : "";
        break;
    case '1':
    default:
        campoA = a->aluno->nome;
        campoB = b->aluno->nome;
        break;
    }

    int comparacao = compararStringsCaseInsensitive(campoA, campoB);
    if (comparacao == 0)
    {
        comparacao = compararStringsCaseInsensitive(a->aluno->nome, b->aluno->nome);
    }
    if (comparacao == 0)
    {
        comparacao = compararStringsCaseInsensitive(a->aluno->id, b->aluno->id);
    }

    return comparacao;
}

static int compararStringsCaseInsensitive(const char *a, const char *b)
{
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        int aa = tolower((unsigned char)a[i]);
        int bb = tolower((unsigned char)b[i]);

        if (aa != bb)
        {
            return aa - bb;
        }
        i++;
    }

    return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]);
}

static const struct plano *buscarPlanoPorIdLocal(const char *id)
{
    if (id == NULL || id[0] == '\0')
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

static const char *descricaoStatusAluno(char status)
{
    switch (status)
    {
    case '1':
        return "Somente ativos";
    case '2':
        return "Somente inativos";
    case '3':
    default:
        return "Todos";
    }
}

static const char *descricaoOrdenacao(char criterio)
{
    switch (criterio)
    {
    case '2':
        return "ID";
    case '3':
        return "Plano";
    case '1':
    default:
        return "Nome";
    }
}

static void exportarAlunosCsv(const struct aluno_view *lista, int total)
{
    FILE *fp = fopen("relatorio_alunos.csv", "w");
    if (fp == NULL)
    {
        printf("\nNao foi possivel criar o arquivo CSV.\n");
        return;
    }

    fprintf(fp, "ID;Nome;Status;PlanoID;PlanoNome\n");
    for (int i = 0; i < total; i++)
    {
        const struct aluno *aluno = lista[i].aluno;
        const char *planoId = aluno->plano_id[0] != '\0' ? aluno->plano_id : "N/A";
        const char *planoNome = lista[i].plano != NULL ? lista[i].plano->nome : "Sem plano";
        fprintf(fp, "%s;%s;%s;%s;%s\n",
                aluno->id,
                aluno->nome,
                aluno->ativo ? "Ativo" : "Inativo",
                planoId,
                planoNome);
    }

    fclose(fp);
    printf("\nArquivo 'relatorio_alunos.csv' gerado com sucesso.\n");
}

static void extrairRegiao(const char *endereco, char *dest, size_t tamanho)
{
    if (dest == NULL || tamanho == 0)
    {
        return;
    }

    if (endereco == NULL || endereco[0] == '\0')
    {
        strncpy(dest, "NAO INFORMADO", tamanho);
        dest[tamanho - 1] = '\0';
        return;
    }

    const char *ultimoHifen = strrchr(endereco, '-');
    const char *inicio = ultimoHifen != NULL ? ultimoHifen + 1 : endereco;

    while (*inicio == ' ' || *inicio == '\t')
    {
        inicio++;
    }

    size_t len = 0;
    while (inicio[len] != '\0' && inicio[len] != '\n' && len + 1 < tamanho)
    {
        dest[len] = (char)toupper((unsigned char)inicio[len]);
        len++;
    }
    dest[len] = '\0';

    while (len > 0 && (dest[len - 1] == ' ' || dest[len - 1] == '\t'))
    {
        dest[len - 1] = '\0';
        len--;
    }

    if (dest[0] == '\0')
    {
        strncpy(dest, "NAO INFORMADO", tamanho);
        dest[tamanho - 1] = '\0';
    }
}
