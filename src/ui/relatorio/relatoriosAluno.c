#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosAluno.h"
#include "utilsRelatorios.h"
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
void relatorioAlunosFiltroAvancado(void);

struct FiltrosAluno
{
    bool filtrarStatus;
    char status;
    bool filtrarPlano;
    char planoId[12];
    bool filtrarIdadeMin;
    int idadeMin;
    bool filtrarIdadeMax;
    int idadeMax;
    bool filtrarRegiao;
    char regiao[128];
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
static void exportarAlunosCsv(const struct aluno_view *lista, int total, char criterio, char filtroStatus, const char *termo);
static void extrairRegiao(const char *endereco, char *dest, size_t tamanho);
static void inicializarFiltrosAluno(struct FiltrosAluno *filtros);
static int aplicarFiltrosAluno(const struct FiltrosAluno *filtros, struct aluno_view *destino, int maxResultados);

static char criterioComparacaoAtual = '1';
static int comparadorAlunoView(const void *a, const void *b);

static struct aluno_view cacheResultadosOrdenados[MAX_ALUNOS];
static int cacheTotalOrdenados = 0;
static char cacheCriterioOrdenacao = '\0';
static char cacheFiltroStatus = '\0';
static char cacheTermo[256] = "";
static time_t cacheTimestampOrdenacao = 0;
static bool cacheOrdenacaoValido = false;

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
        printf("===  [7]  FILTRO AVANCADO (MULTIPLOS FILTROS)                 ===\n");
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

        case '7':
            relatorioAlunosFiltroAvancado();
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

    bool reutilizarCache = cacheOrdenacaoValido &&
                           cacheCriterioOrdenacao == ordenacao &&
                           cacheFiltroStatus == filtroStatus &&
                           cacheTotalOrdenados == totalFiltrados &&
                           strcmp(cacheTermo, termo) == 0;

    // Cache simples: se filtros/ordenacao iguais, reaproveita lista ordenada da última execução
    if (reutilizarCache)
    {
        memcpy(resultados, cacheResultadosOrdenados, sizeof(struct aluno_view) * cacheTotalOrdenados);
    }
    else
    {
        ordenarResultados(resultados, totalFiltrados, ordenacao);
        memcpy(cacheResultadosOrdenados, resultados, sizeof(struct aluno_view) * totalFiltrados);
        cacheTotalOrdenados = totalFiltrados;
        cacheCriterioOrdenacao = ordenacao;
        cacheFiltroStatus = filtroStatus;
        strncpy(cacheTermo, termo, sizeof(cacheTermo));
        cacheTermo[sizeof(cacheTermo) - 1] = '\0';
        cacheTimestampOrdenacao = time(NULL);
        cacheOrdenacaoValido = true;
    }

    int somaIdades = 0;
    int idadesValidas = 0;
    int idadeMin = INT_MAX;
    int idadeMax = INT_MIN;
    int ativos = 0;
    int inativos = 0;
    struct PlanoFreq
    {
        const char *nome;
        int quantidade;
    } planosMaisComuns[MAX_ALUNOS];
    int totalPlanosContados = 0;

    for (int i = 0; i < totalFiltrados; i++)
    {
        const struct aluno *aluno = resultados[i].aluno;
        int idade = calcularIdade(aluno->idade);
        if (idade > 0)
        {
            somaIdades += idade;
            idadesValidas++;
            if (idade < idadeMin)
            {
                idadeMin = idade;
            }
            if (idade > idadeMax)
            {
                idadeMax = idade;
            }
        }

        if (aluno->ativo)
        {
            ativos++;
        }
        else
        {
            inativos++;
        }

        const char *planoNome = resultados[i].plano != NULL ? resultados[i].plano->nome : "Sem plano";
        int encontrado = -1;
        for (int p = 0; p < totalPlanosContados; p++)
        {
            if (strcmp(planosMaisComuns[p].nome, planoNome) == 0)
            {
                encontrado = p;
                break;
            }
        }
        if (encontrado == -1 && totalPlanosContados < MAX_ALUNOS)
        {
            planosMaisComuns[totalPlanosContados].nome = planoNome;
            planosMaisComuns[totalPlanosContados].quantidade = 1;
            totalPlanosContados++;
        }
        else if (encontrado != -1)
        {
            planosMaisComuns[encontrado].quantidade++;
        }
    }

    printf("\nStatus: %s | Ordenacao: %s | Termo: %s\n",
           descricaoStatusAluno(filtroStatus),
           descricaoOrdenacao(ordenacao),
           termo[0] != '\0' ? termo : "Nenhum");
    const char *setaOrdenacao = (ordenacao == '4' || ordenacao == '6') ? "▼" : "▲";
    printf(">>> Ordenando por %s %s\n", descricaoOrdenacao(ordenacao), setaOrdenacao);
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

    double idadeMedia = idadesValidas > 0 ? (double)somaIdades / idadesValidas : 0.0;
    const char *planoMaisComum = "N/A";
    int qtdPlanoMaisComum = 0;
    for (int p = 0; p < totalPlanosContados; p++)
    {
        if (planosMaisComuns[p].quantidade > qtdPlanoMaisComum)
        {
            qtdPlanoMaisComum = planosMaisComuns[p].quantidade;
            planoMaisComum = planosMaisComuns[p].nome;
        }
    }

    printf("\n-------------------------------------------------------------------------\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("|                     ESTATISTICAS DOS RESULTADOS                       |\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("| Total exibido: %-55d |\n", totalFiltrados);
    if (idadesValidas > 0)
    {
        printf("| Idade media: %-6.2f | Idade minima: %-3d | Idade maxima: %-3d               |\n", idadeMedia, idadeMin, idadeMax);
    }
    else
    {
        printf("| Idade media: N/A   | Idade minima: N/A | Idade maxima: N/A               |\n");
    }
    printf("| Plano mais comum: %-20.20s (%-3d)                               |\n", planoMaisComum, qtdPlanoMaisComum);
    printf("| Status: Ativos %-4d | Inativos %-4d                                      |\n", ativos, inativos);
    printf("+-----------------------------------------------------------------------+\n");
    printf("-------------------------------------------------------------------------\n");

    printf("Deseja exportar o resultado para CSV? (S/N): ");
    int resposta = getchar();
    while (resposta == '\n')
    {
        resposta = getchar();
    }
    if (resposta == 's' || resposta == 'S')
    {
        exportarAlunosCsv(resultados, totalFiltrados, ordenacao, filtroStatus, termo);
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

void relatorioAlunosFiltroAvancado(void)
{
    struct FiltrosAluno filtros;
    inicializarFiltrosAluno(&filtros);

    while (1)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===         RELATORIO - ALUNOS COM FILTROS AVANCADOS                 ===\n");
        printf("=========================================================================\n");

        printf("Filtros ativos:\n");
        bool temFiltro = false;
        if (filtros.filtrarStatus)
        {
            printf(" - Status: %s\n", filtros.status == '1' ? "Ativos" : (filtros.status == '2' ? "Inativos" : "Todos"));
            temFiltro = true;
        }
        if (filtros.filtrarPlano && filtros.planoId[0] != '\0')
        {
            printf(" - Plano: %s\n", filtros.planoId);
            temFiltro = true;
        }
        if (filtros.filtrarIdadeMin)
        {
            printf(" - Idade minima: %d\n", filtros.idadeMin);
            temFiltro = true;
        }
        if (filtros.filtrarIdadeMax)
        {
            printf(" - Idade maxima: %d\n", filtros.idadeMax);
            temFiltro = true;
        }
        if (filtros.filtrarRegiao && filtros.regiao[0] != '\0')
        {
            printf(" - Regiao: %s\n", filtros.regiao);
            temFiltro = true;
        }
        if (!temFiltro)
        {
            printf(" Nenhum filtro ativo.\n");
        }

        printf("\nSelecione uma opcao:\n");
        printf("[1] Adicionar filtro de status\n");
        printf("[2] Adicionar filtro de plano\n");
        printf("[3] Adicionar filtro de idade minima\n");
        printf("[4] Adicionar filtro de idade maxima\n");
        printf("[5] Adicionar filtro de regiao\n");
        printf("[6] Limpar todos os filtros\n");
        printf("[7] Aplicar filtros e gerar relatorio\n");
        printf("[0] Voltar\n");

        char op = lerTecla();
        switch (op)
        {
        case '1':
            filtros.status = selecionarFiltroStatus();
            filtros.filtrarStatus = true;
            break;
        case '2':
        {
            printf("\nDigite o ID do plano (deixe vazio para remover o filtro): ");
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0')
                {
                    filtros.filtrarPlano = false;
                    filtros.planoId[0] = '\0';
                }
                else
                {
                    filtros.filtrarPlano = true;
                    strncpy(filtros.planoId, buffer, sizeof(filtros.planoId));
                    filtros.planoId[sizeof(filtros.planoId) - 1] = '\0';
                }
            }
            break;
        }
        case '3':
        {
            printf("\nInforme a idade minima (vazio para remover o filtro): ");
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0')
                {
                    filtros.filtrarIdadeMin = false;
                    filtros.idadeMin = 0;
                }
                else
                {
                    filtros.filtrarIdadeMin = true;
                    filtros.idadeMin = atoi(buffer);
                }
            }
            break;
        }
        case '4':
        {
            printf("\nInforme a idade maxima (vazio para remover o filtro): ");
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0')
                {
                    filtros.filtrarIdadeMax = false;
                    filtros.idadeMax = 0;
                }
                else
                {
                    filtros.filtrarIdadeMax = true;
                    filtros.idadeMax = atoi(buffer);
                }
            }
            break;
        }
        case '5':
        {
            printf("\nInforme a regiao (vazio para remover o filtro): ");
            char buffer[128];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0')
                {
                    filtros.filtrarRegiao = false;
                    filtros.regiao[0] = '\0';
                }
                else
                {
                    filtros.filtrarRegiao = true;
                    strncpy(filtros.regiao, buffer, sizeof(filtros.regiao));
                    filtros.regiao[sizeof(filtros.regiao) - 1] = '\0';
                }
            }
            break;
        }
        case '6':
            inicializarFiltrosAluno(&filtros);
            printf("\nFiltros limpos. Pressione <ENTER> para continuar...");
            getchar();
            break;
        case '7':
        {
            struct aluno_view resultados[MAX_ALUNOS];
            int total = aplicarFiltrosAluno(&filtros, resultados, MAX_ALUNOS);

            limparTela();
            printf("=========================================================================\n");
            printf("===                 RESULTADO - FILTROS AVANCADOS                    ===\n");
            printf("=========================================================================\n");
            if (total == 0)
            {
                printf("Nenhum aluno encontrado com os filtros atuais.\n");
            }
            else
            {
                const int porPagina = 10;
                int totalPaginas = (total + porPagina - 1) / porPagina;

                for (int pagina = 0; pagina < totalPaginas; pagina++)
                {
                    int inicio = pagina * porPagina;
                    int fim = inicio + porPagina;
                    if (fim > total)
                    {
                        fim = total;
                    }

                    printf("\n--- Pagina %d de %d ----------------------------------------------\n", pagina + 1, totalPaginas);
                    for (int i = inicio; i < fim; i++)
                    {
                        const struct aluno *aluno = resultados[i].aluno;
                        const struct plano *plano = resultados[i].plano;
                        int idade = calcularIdade(aluno->idade);
                        char idadeStr[16];
                        if (idade > 0)
                        {
                            snprintf(idadeStr, sizeof(idadeStr), "%d", idade);
                        }
                        else
                        {
                            strncpy(idadeStr, "N/A", sizeof(idadeStr));
                            idadeStr[sizeof(idadeStr) - 1] = '\0';
                        }
                        char regiaoAluno[128];
                        extrairRegiao(aluno->endereco, regiaoAluno, sizeof(regiaoAluno));

                        printf("[%s] %s\n", aluno->id, aluno->nome);
                        printf("    Status: %s | Idade: %s | Regiao: %s\n",
                               aluno->ativo ? "Ativo" : "Inativo",
                               idadeStr,
                               regiaoAluno);
                        printf("    Plano: %s (%s)\n",
                               plano != NULL ? plano->nome : "Sem plano",
                               (aluno->plano_id[0] != '\0') ? aluno->plano_id : "N/A");
                    }

                    if (pagina < totalPaginas - 1)
                    {
                        printf("\nPressione ENTER para proxima pagina...");
                        getchar();
                    }
                }
                printf("\nFim dos resultados\n");
            }
            printf("\nTotal de alunos listados: %d\n", total);
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para voltar ao menu de filtros...");
            getchar();
            break;
        }
        case '0':
            limparTela();
            return;
        default:
            opInvalida();
            break;
        }
    }
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
        printf("[4] Nome (ordem reversa Z-A)\n");
        printf("[5] Idade (menor para maior)\n");
        printf("[6] Idade (maior para menor)\n");

        char op = lerTecla();
        if (op >= '1' && op <= '6')
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

static void inicializarFiltrosAluno(struct FiltrosAluno *filtros)
{
    if (filtros == NULL)
    {
        return;
    }

    filtros->filtrarStatus = false;
    filtros->status = '0';
    filtros->filtrarPlano = false;
    filtros->planoId[0] = '\0';
    filtros->filtrarIdadeMin = false;
    filtros->idadeMin = 0;
    filtros->filtrarIdadeMax = false;
    filtros->idadeMax = 0;
    filtros->filtrarRegiao = false;
    filtros->regiao[0] = '\0';
}

static bool alunoAtendeFiltros(const struct aluno *aluno, const struct plano *plano, const struct FiltrosAluno *filtros)
{
    if (aluno == NULL || filtros == NULL)
    {
        return false;
    }

    if (filtros->filtrarStatus)
    {
        switch (filtros->status)
        {
        case '1':
            if (!aluno->ativo)
            {
                return false;
            }
            break;
        case '2':
            if (aluno->ativo)
            {
                return false;
            }
            break;
        default:
            break;
        }
    }

    if (filtros->filtrarPlano)
    {
        if (filtros->planoId[0] == '\0' || strcmp(aluno->plano_id, filtros->planoId) != 0)
        {
            return false;
        }
    }

    if (filtros->filtrarIdadeMin || filtros->filtrarIdadeMax)
    {
        int idade = calcularIdade(aluno->idade);
        if (idade <= 0)
        {
            return false;
        }
        if (filtros->filtrarIdadeMin && idade < filtros->idadeMin)
        {
            return false;
        }
        if (filtros->filtrarIdadeMax && idade > filtros->idadeMax)
        {
            return false;
        }
    }

    if (filtros->filtrarRegiao)
    {
        if (filtros->regiao[0] == '\0')
        {
            return false;
        }

        char regiaoAluno[128];
        extrairRegiao(aluno->endereco, regiaoAluno, sizeof(regiaoAluno));
        if (compararStringsCaseInsensitive(regiaoAluno, filtros->regiao) != 0)
        {
            return false;
        }
    }

    (void)plano;
    return true;
}

static int aplicarFiltrosAluno(const struct FiltrosAluno *filtros, struct aluno_view *destino, int maxResultados)
{
    if (filtros == NULL || destino == NULL || maxResultados <= 0)
    {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < total_alunos && total < maxResultados; i++)
    {
        const struct plano *plano = buscarPlanoPorIdLocal(lista_alunos[i].plano_id);
        if (alunoAtendeFiltros(&lista_alunos[i], plano, filtros))
        {
            destino[total].aluno = &lista_alunos[i];
            destino[total].plano = plano;
            total++;
        }
    }

    return total;
}

static void ordenarResultados(struct aluno_view *lista, int total, char criterio)
{
    criterioComparacaoAtual = criterio;
    // Heurística: usa insertion sort para listas pequenas, qsort para listas maiores.
    ordenarAlunosOtimizado(lista, total, comparadorAlunoView);
}

static int compararViews(const struct aluno_view *a, const struct aluno_view *b, char criterio)
{
    int comparacao = 0;

    switch (criterio)
    {
    case '2':
        comparacao = compararStringsCaseInsensitive(a->aluno->id, b->aluno->id);
        break;
    case '3':
    {
        const char *planoA = a->plano != NULL ? a->plano->nome : "";
        const char *planoB = b->plano != NULL ? b->plano->nome : "";
        comparacao = compararStringsCaseInsensitive(planoA, planoB);
        break;
    }
    case '4':
        comparacao = compararStringsCaseInsensitive(b->aluno->nome, a->aluno->nome);
        break;
    case '5':
    {
        int idadeA = calcularIdade(a->aluno->idade);
        int idadeB = calcularIdade(b->aluno->idade);
        comparacao = idadeA - idadeB;
        break;
    }
    case '6':
    {
        int idadeA = calcularIdade(a->aluno->idade);
        int idadeB = calcularIdade(b->aluno->idade);
        comparacao = idadeB - idadeA;
        break;
    }
    case '1':
    default:
        comparacao = compararStringsCaseInsensitive(a->aluno->nome, b->aluno->nome);
        break;
    }

    // Ordenacao em cascata: se empatar no criterio principal, desempata por nome e depois por ID
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

static int comparadorAlunoView(const void *a, const void *b)
{
    const struct aluno_view *va = (const struct aluno_view *)a;
    const struct aluno_view *vb = (const struct aluno_view *)b;
    return compararViews(va, vb, criterioComparacaoAtual);
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
    case '4':
        return "Nome (ordem reversa Z-A)";
    case '5':
        return "Idade (menor para maior)";
    case '6':
        return "Idade (maior para menor)";
    case '1':
    default:
        return "Nome";
    }
}

static const char *slugOrdenacao(char criterio)
{
    switch (criterio)
    {
    case '2':
        return "ordenado_id";
    case '3':
        return "ordenado_plano";
    case '4':
        return "ordenado_nome_reverso";
    case '5':
        return "ordenado_idade_crescente";
    case '6':
        return "ordenado_idade_decrescente";
    case '1':
    default:
        return "ordenado_nome";
    }
}

static void exportarAlunosCsv(const struct aluno_view *lista, int total, char criterio, char filtroStatus, const char *termo)
{
    time_t agora = time(NULL);
    char timestampStr[32];
    snprintf(timestampStr, sizeof(timestampStr), "%ld", (long)agora);

    char nomeArquivo[160];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "relatorio_alunos_%s_%s.csv", slugOrdenacao(criterio), timestampStr);

    FILE *fp = fopen(nomeArquivo, "w");
    if (fp == NULL)
    {
        printf("\nNao foi possivel criar o arquivo CSV.\n");
        return;
    }

    char dataHora[64];
    struct tm *tm_info = localtime(&agora);
    if (tm_info != NULL)
    {
        strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", tm_info);
    }
    else
    {
        strncpy(dataHora, "N/A", sizeof(dataHora));
        dataHora[sizeof(dataHora) - 1] = '\0';
    }

    char filtros[256];
    const char *statusDesc = descricaoStatusAluno(filtroStatus);
    if (termo != NULL && termo[0] != '\0')
    {
        snprintf(filtros, sizeof(filtros), "Status=%s;Termo=%s", statusDesc, termo);
    }
    else
    {
        snprintf(filtros, sizeof(filtros), "Status=%s;Termo=Nenhum", statusDesc);
    }

    fprintf(fp, "# DataHora;%s\n", dataHora);
    fprintf(fp, "# Ordenacao;%s\n", descricaoOrdenacao(criterio));
    fprintf(fp, "# Registros;%d\n", total);
    fprintf(fp, "# Filtros;%s\n", filtros);

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
    printf("\nArquivo '%s' gerado com sucesso.\n", nomeArquivo);
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
