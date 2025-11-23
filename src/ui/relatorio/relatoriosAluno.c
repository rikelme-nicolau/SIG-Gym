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
#include "ui/utils/consoleLayout.h"

#define LIST_COL_ID 8
#define LIST_COL_NOME 18
#define LIST_COL_PLANO 14
#define LIST_COL_STATUS 8
#define LIST_COL_REGIAO 12

#define SEM_PLANO_COL_ID 8
#define SEM_PLANO_COL_NOME 18
#define SEM_PLANO_COL_TEL 14
#define SEM_PLANO_COL_EMAIL 14
#define SEM_PLANO_COL_STATUS 6

#define STATUS_COL_ID 8
#define STATUS_COL_NOME 30
#define STATUS_COL_PLANO 28

#define PLANO_COL_NOME 20
#define PLANO_COL_HORARIO 12
#define PLANO_COL_ALUNOS 8
#define PLANO_COL_VALOR 10
#define PLANO_COL_RECEITA 10

#define REGIAO_COL_NOME 40
#define REGIAO_COL_QTD 12
#define REGIAO_COL_PCT 14

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

static void cabecalho_relatorio(const char *subtitulo);
static void aguardar_voltar(void);
static void tabela_listagem_header(void);
static void tabela_listagem_row(const struct aluno *aluno, const struct plano *plano);
static void mostrar_filtros_ativos(const struct FiltrosAluno *filtros);
static void exibir_listagem_paginada(const struct aluno_view *lista, int total);
static void tabela_sem_plano_header(void);
static void tabela_sem_plano_row(const struct aluno *aluno);
static void tabela_status_header(const char *titulo);
static void tabela_status_row(const struct aluno *aluno);

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
    ui_append_col(linha, sizeof(linha), &pos, "Plano", LIST_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", LIST_COL_STATUS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Regiao", LIST_COL_REGIAO);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_listagem_row(const struct aluno *aluno, const struct plano *plano)
{
    char linha[UI_INNER + 1];
    char regiao[64];
    extrairRegiao(aluno->endereco, regiao, sizeof(regiao));

    char planoInfo[96];
    char planoNome[40];
    char planoId[24];
    const char *planoOrigem = plano != NULL ? plano->nome : "Sem plano";
    const char *planoCodigo = aluno->plano_id[0] != '\0' ? aluno->plano_id : "N/A";
    ui_clip_utf8(planoOrigem, 28, planoNome, sizeof(planoNome));
    ui_clip_utf8(planoCodigo, 16, planoId, sizeof(planoId));
    snprintf(planoInfo, sizeof(planoInfo), "%s (%s)", planoNome, planoId);

    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, aluno->id, LIST_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, aluno->nome, LIST_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, planoInfo, LIST_COL_PLANO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, aluno->ativo ? "Ativo" : "Inativo", LIST_COL_STATUS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, regiao, LIST_COL_REGIAO);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void mostrar_filtros_ativos(const struct FiltrosAluno *filtros)
{
    ui_section_title("Filtros ativos");
    bool temFiltro = false;
    char linha[UI_INNER + 1];

    if (filtros->filtrarStatus)
    {
        snprintf(linha, sizeof(linha), "Status: %s", descricaoStatusAluno(filtros->status));
        ui_text_line(linha);
        temFiltro = true;
    }
    if (filtros->filtrarPlano && filtros->planoId[0] != '\0')
    {
        snprintf(linha, sizeof(linha), "Plano: %s", filtros->planoId);
        ui_text_line(linha);
        temFiltro = true;
    }
    if (filtros->filtrarIdadeMin)
    {
        snprintf(linha, sizeof(linha), "Idade minima: %d", filtros->idadeMin);
        ui_text_line(linha);
        temFiltro = true;
    }
    if (filtros->filtrarIdadeMax)
    {
        snprintf(linha, sizeof(linha), "Idade maxima: %d", filtros->idadeMax);
        ui_text_line(linha);
        temFiltro = true;
    }
    if (filtros->filtrarRegiao && filtros->regiao[0] != '\0')
    {
        snprintf(linha, sizeof(linha), "Regiao: %s", filtros->regiao);
        ui_text_line(linha);
        temFiltro = true;
    }

    if (!temFiltro)
    {
        ui_center_text("Nenhum filtro ativo.");
    }
    ui_line('-');
}

static void exibir_listagem_paginada(const struct aluno_view *lista, int total)
{
    const int porPagina = 10;
    int totalPaginas = (total + porPagina - 1) / porPagina;

    for (int pagina = 0; pagina < totalPaginas; pagina++)
    {
        char titulo[64];
        snprintf(titulo, sizeof(titulo), "Pagina %d de %d", pagina + 1, totalPaginas);
        ui_section_title(titulo);
        tabela_listagem_header();

        int inicio = pagina * porPagina;
        int fim = inicio + porPagina;
        if (fim > total)
        {
            fim = total;
        }

        for (int i = inicio; i < fim; i++)
        {
            tabela_listagem_row(lista[i].aluno, lista[i].plano);
        }

        ui_line('-');

        if (pagina < totalPaginas - 1)
        {
            ui_section_title("Pressione <ENTER> para continuar");
            getchar();
        }
    }
}

static void tabela_sem_plano_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", SEM_PLANO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", SEM_PLANO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Telefone", SEM_PLANO_COL_TEL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Email", SEM_PLANO_COL_EMAIL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", SEM_PLANO_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_sem_plano_row(const struct aluno *aluno)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, aluno->id, SEM_PLANO_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, aluno->nome, SEM_PLANO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos,
                  aluno->telefone[0] != '\0' ? aluno->telefone : "Nao informado",
                  SEM_PLANO_COL_TEL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos,
                  aluno->email[0] != '\0' ? aluno->email : "Nao informado",
                  SEM_PLANO_COL_EMAIL);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, aluno->ativo ? "Ativo" : "Inativo", SEM_PLANO_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

static void tabela_status_header(const char *titulo)
{
    ui_section_title(titulo);
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", STATUS_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", STATUS_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", STATUS_COL_PLANO);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_status_row(const struct aluno *aluno)
{
    char linha[UI_INNER + 1];
    char planoInfo[96] = "Nao informado";
    if (aluno->plano_id[0] != '\0')
    {
        const struct plano *plano = buscarPlanoPorIdLocal(aluno->plano_id);
        const char *nomeOrigem = plano != NULL ? plano->nome : aluno->plano_id;
        char nomeClip[40];
        char codigoClip[24];
        ui_clip_utf8(nomeOrigem, 28, nomeClip, sizeof(nomeClip));
        ui_clip_utf8(aluno->plano_id, 16, codigoClip, sizeof(codigoClip));
        snprintf(planoInfo, sizeof(planoInfo), "%s (%s)", nomeClip, codigoClip);
    }

    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, aluno->id, STATUS_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, aluno->nome, STATUS_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, planoInfo, STATUS_COL_PLANO);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void moduloRelatoriosAluno(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios de Alunos");
        ui_empty_line();
        ui_menu_option('1', "Listagem completa (com filtros)");
        ui_menu_option('2', "Alunos por plano");
        ui_menu_option('3', "Alunos por faixa etaria");
        ui_menu_option('4', "Alunos por regiao");
        ui_menu_option('5', "Alunos ativos vs inativos");
        ui_menu_option('6', "Alunos sem plano definido");
        ui_menu_option('7', "Filtro avancado (multiplos filtros)");
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
    cabecalho_relatorio("Relatorio - Listagem completa de alunos");
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
        ui_section_title("Nenhum aluno encontrado");
        ui_text_line("Ajuste os filtros e tente novamente.");
        aguardar_voltar();
        return;
    }

    bool reutilizarCache = cacheOrdenacaoValido &&
                           cacheCriterioOrdenacao == ordenacao &&
                           cacheFiltroStatus == filtroStatus &&
                           cacheTotalOrdenados == totalFiltrados &&
                           strcmp(cacheTermo, termo) == 0;

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

    cabecalho_relatorio("Relatorio - Listagem completa de alunos");

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

    ui_section_title("Filtros aplicados");
    char linha[UI_INNER + 1];
    char termoCurto[32];
    const char *termoOrigem = termo[0] != '\0' ? termo : "Nenhum";
    ui_clip_utf8(termoOrigem, 28, termoCurto, sizeof(termoCurto));
    snprintf(linha, sizeof(linha), "Status: %s | Ordenacao: %s",
             descricaoStatusAluno(filtroStatus),
             descricaoOrdenacao(ordenacao));
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Termo: %s", termoCurto);
    ui_text_line(linha);
    const char *setaOrdenacao = (ordenacao == '4' || ordenacao == '6') ? "▼" : "▲";
    snprintf(linha, sizeof(linha), "Ordenando por %s %s", descricaoOrdenacao(ordenacao), setaOrdenacao);
    ui_text_line(linha);
    ui_line('-');

    exibir_listagem_paginada(resultados, totalFiltrados);

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

    ui_section_title("Estatisticas dos resultados");
    snprintf(linha, sizeof(linha), "Total exibido: %d | Ativos: %d | Inativos: %d", totalFiltrados, ativos, inativos);
    ui_text_line(linha);
    if (idadesValidas > 0)
    {
        snprintf(linha, sizeof(linha), "Idade media: %.2f | Minima: %d | Maxima: %d", idadeMedia, idadeMin, idadeMax);
        ui_text_line(linha);
    }
    else
    {
        ui_text_line("Idade media: N/A | Minima: N/A | Maxima: N/A");
    }
    snprintf(linha, sizeof(linha), "Plano mais comum: %s (%d)", planoMaisComum, qtdPlanoMaisComum);
    ui_text_line(linha);
    ui_line('-');

    ui_section_title("Exportar resultado");
    ui_text_line("Deseja exportar o resultado para CSV? (S/N)");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
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

    aguardar_voltar();
}

void relatorioAlunosFiltroAvancado(void)
{
    struct FiltrosAluno filtros;
    inicializarFiltrosAluno(&filtros);

    while (1)
    {
        cabecalho_relatorio("Relatorio - Alunos com filtros avancados");
        mostrar_filtros_ativos(&filtros);
        ui_menu_option('1', "Adicionar filtro de status");
        ui_menu_option('2', "Adicionar filtro de plano");
        ui_menu_option('3', "Adicionar filtro de idade minima");
        ui_menu_option('4', "Adicionar filtro de idade maxima");
        ui_menu_option('5', "Adicionar filtro de regiao");
        ui_menu_option('6', "Limpar todos os filtros");
        ui_menu_option('7', "Aplicar filtros e gerar relatorio");
        ui_empty_line();
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

        char op = lerTecla();
        limparTela();

        switch (op)
        {
        case '1':
            filtros.status = selecionarFiltroStatus();
            filtros.filtrarStatus = true;
            break;
        case '2':
        {
            cabecalho_relatorio("Filtro - Plano");
            ui_text_line("Digite o ID do plano (vazio para remover o filtro).");
            ui_line('=');
            printf(">>> ");
            fflush(stdout);
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
            cabecalho_relatorio("Filtro - Idade minima");
            ui_text_line("Informe a idade minima (vazio para remover o filtro).");
            ui_line('=');
            printf(">>> ");
            fflush(stdout);
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
            cabecalho_relatorio("Filtro - Idade maxima");
            ui_text_line("Informe a idade maxima (vazio para remover o filtro).");
            ui_line('=');
            printf(">>> ");
            fflush(stdout);
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
            cabecalho_relatorio("Filtro - Regiao");
            ui_text_line("Informe a regiao (vazio para remover o filtro).");
            ui_line('=');
            printf(">>> ");
            fflush(stdout);
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
            cabecalho_relatorio("Relatorio - Alunos com filtros avancados");
            ui_center_text("Todos os filtros foram limpos.");
            ui_section_title("Pressione <ENTER> para continuar");
            getchar();
            break;
        case '7':
        {
            struct aluno_view resultados[MAX_ALUNOS];
            int total = aplicarFiltrosAluno(&filtros, resultados, MAX_ALUNOS);

            cabecalho_relatorio("Resultado - Filtros avancados");
            if (total == 0)
            {
                ui_section_title("Nenhum aluno encontrado");
                ui_text_line("Ajuste os filtros e tente novamente.");
                ui_section_title("Pressione <ENTER> para voltar");
                getchar();
                limparTela();
                break;
            }

            exibir_listagem_paginada(resultados, total);
            ui_section_title("Pressione <ENTER> para voltar aos filtros");
            getchar();
            limparTela();
            break;
        }
        case '0':
            return;
        default:
            opInvalida();
            break;
        }
    }
}

void relatorioAlunosPorPlano(void)
{
    cabecalho_relatorio("Relatorio - Alunos por plano");

    if (total_planos == 0)
    {
        ui_center_text("Nao ha planos cadastrados.");
        aguardar_voltar();
        return;
    }

    double receitaTotal = 0.0;
    int totalAlunos = 0;

    ui_section_title("Resumo por plano");
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Plano", PLANO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Horario", PLANO_COL_HORARIO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Alunos", PLANO_COL_ALUNOS);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Valor", PLANO_COL_VALOR);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Receita", PLANO_COL_RECEITA);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');

    for (int i = 0; i < total_planos; i++)
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

        double receitaPlano = alunosPlano * lista_planos[i].valor;
        receitaTotal += receitaPlano;
        totalAlunos += alunosPlano;

        char horario[32];
        snprintf(horario, sizeof(horario), "%s-%s", lista_planos[i].horario_inicio, lista_planos[i].horario_fim);
        char alunosStr[16];
        snprintf(alunosStr, sizeof(alunosStr), "%d", alunosPlano);
        char valorStr[32];
        snprintf(valorStr, sizeof(valorStr), "R$ %.2f", lista_planos[i].valor);
        char receitaStr[32];
        snprintf(receitaStr, sizeof(receitaStr), "R$ %.2f", receitaPlano);

        pos = 0;
        ui_append_col(linha, sizeof(linha), &pos, lista_planos[i].nome, PLANO_COL_NOME);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, horario, PLANO_COL_HORARIO);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, alunosStr, PLANO_COL_ALUNOS);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, valorStr, PLANO_COL_VALOR);
        ui_append_sep(linha, sizeof(linha), &pos);
        ui_append_col(linha, sizeof(linha), &pos, receitaStr, PLANO_COL_RECEITA);
        linha[pos] = '\0';
        ui_text_line(linha);
    }

    ui_line('-');
    ui_section_title("Detalhamento por plano");

    for (int i = 0; i < total_planos; i++)
    {
        if (!lista_planos[i].ativo)
        {
            continue;
        }

        ui_section_title(lista_planos[i].nome);
        char info[UI_INNER + 1];
        snprintf(info, sizeof(info), "Valor: R$ %.2f | Horario: %s - %s", lista_planos[i].valor, lista_planos[i].horario_inicio, lista_planos[i].horario_fim);
        ui_text_line(info);

        int alunosPlano = 0;
        for (int j = 0; j < total_alunos; j++)
        {
            if (!lista_alunos[j].ativo)
            {
                continue;
            }

            if (strcmp(lista_alunos[j].plano_id, lista_planos[i].id) == 0)
            {
                if (alunosPlano == 0)
                {
                    ui_text_line("Alunos vinculados:");
                }
                char alunoLinha[128];
                char nomeAlunoClip[64];
                ui_clip_utf8(lista_alunos[j].nome, 60, nomeAlunoClip, sizeof(nomeAlunoClip));
                snprintf(alunoLinha, sizeof(alunoLinha), "- [%.16s] %.60s", lista_alunos[j].id, nomeAlunoClip);
                ui_text_line(alunoLinha);
                alunosPlano++;
            }
        }

        if (alunosPlano == 0)
        {
            ui_text_line("Nenhum aluno vinculado a este plano.");
        }

        ui_line('-');
    }

    ui_section_title("Totais gerais");
    snprintf(linha, sizeof(linha), "Total de alunos vinculados: %d", totalAlunos);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Receita total estimada: R$ %.2f", receitaTotal);
    ui_text_line(linha);
    ui_line('-');

    aguardar_voltar();
}

void relatorioAlunosPorFaixaEtaria(void)
{
    cabecalho_relatorio("Relatorio - Alunos por faixa etaria");

    if (total_alunos == 0)
    {
        ui_center_text("Nao ha alunos cadastrados.");
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

    int contadores[TOTAL_FAIXAS] = {0};
    const char *nomesFaixa[TOTAL_FAIXAS][MAX_ALUNOS];

    bool listarNomes = false;

    ui_text_line("Deseja listar os nomes em cada faixa? (S/N)");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
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
        ui_section_title("Dados insuficientes");
        ui_text_line("Nao foi possivel calcular idades validas para os alunos.");
        aguardar_voltar();
        return;
    }

    ui_section_title("Distribuicao por faixa");
    for (int i = 0; i < TOTAL_FAIXAS; i++)
    {
        double percentual = (double)contadores[i] / alunosValidos * 100.0;
        int barras = (int)(percentual / 5); // barra maxima 20 caracteres
        if (barras > 20)
        {
            barras = 20;
        }
        char grafico[21];
        for (int b = 0; b < 20; b++)
        {
            grafico[b] = b < barras ? '=' : '.';
        }
        grafico[20] = '\0';

        char linha[UI_INNER + 1];
        snprintf(linha, sizeof(linha), "%-10s: %s (%d aluno%s - %.2f%%)",
                 labels[i],
                 grafico,
                 contadores[i],
                 contadores[i] == 1 ? "" : "s",
                 percentual);
        ui_text_line(linha);

        if (listarNomes && contadores[i] > 0)
        {
            ui_text_line("Nomes:");
            for (int j = 0; j < contadores[i]; j++)
            {
                char nomeLinha[UI_INNER + 1];
                snprintf(nomeLinha, sizeof(nomeLinha), "- %s", nomesFaixa[i][j]);
                ui_text_line(nomeLinha);
            }
        }
    }

    char resumo[UI_INNER + 1];
    snprintf(resumo, sizeof(resumo), "Total de alunos considerados: %d", alunosValidos);
    ui_line('-');
    ui_text_line(resumo);
    ui_line('-');
    aguardar_voltar();
}

void relatorioAlunosPorRegiao(void)
{
    cabecalho_relatorio("Relatorio - Alunos por regiao");

    if (total_alunos == 0)
    {
        ui_center_text("Nao ha alunos cadastrados.");
        aguardar_voltar();
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
        ui_section_title("Dados insuficientes");
        ui_text_line("Nao foi possivel identificar regioes nos enderecos cadastrados.");
        aguardar_voltar();
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

    ui_section_title("Distribuicao por regiao");
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "Regiao", REGIAO_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Quantidade", REGIAO_COL_QTD);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "%", REGIAO_COL_PCT);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');

    for (int i = 0; i < totalRegioes; i++)
    {
        double percentual = (double)regioes[i].quantidade / totalConsiderados * 100.0;
        pos = 0;
        ui_append_col(linha, sizeof(linha), &pos, regioes[i].nome, REGIAO_COL_NOME);
        ui_append_sep(linha, sizeof(linha), &pos);
        char qtdStr[16];
        snprintf(qtdStr, sizeof(qtdStr), "%d", regioes[i].quantidade);
        ui_append_col(linha, sizeof(linha), &pos, qtdStr, REGIAO_COL_QTD);
        ui_append_sep(linha, sizeof(linha), &pos);
        char pctStr[16];
        snprintf(pctStr, sizeof(pctStr), "%.2f", percentual);
        ui_append_col(linha, sizeof(linha), &pos, pctStr, REGIAO_COL_PCT);
        linha[pos] = '\0';
        ui_text_line(linha);
    }

    ui_line('-');
    snprintf(linha, sizeof(linha), "Total de alunos considerados: %d", totalConsiderados);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

void relatorioAlunosAtivosVsInativos(void)
{
    cabecalho_relatorio("Relatorio - Alunos ativos vs inativos");

    if (total_alunos == 0)
    {
        ui_center_text("Nao ha alunos cadastrados.");
        aguardar_voltar();
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

    tabela_status_header("Alunos ativos");
    if (totalAtivos == 0)
    {
        ui_center_text("Nenhum aluno ativo.");
    }
    for (int i = 0; i < totalAtivos; i++)
    {
        tabela_status_row(ativos[i]);
    }

    tabela_status_header("Alunos inativos");
    if (totalInativos == 0)
    {
        ui_center_text("Nenhum aluno inativo.");
    }
    for (int i = 0; i < totalInativos; i++)
    {
        tabela_status_row(inativos[i]);
    }

    ui_section_title("Resumo");
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Ativos: %d | Inativos: %d", totalAtivos, totalInativos);
    ui_text_line(linha);
    snprintf(linha, sizeof(linha), "Taxa de retencao: %.2f%% | Taxa de cancelamento: %.2f%%", taxaRetencao, taxaCancelamento);
    ui_text_line(linha);
    ui_line('-');
    aguardar_voltar();
}

void relatorioAlunosSemPlano(void)
{
    cabecalho_relatorio("Relatorio - Alunos sem plano definido");

    if (total_alunos == 0)
    {
        ui_center_text("Nao ha alunos cadastrados.");
        aguardar_voltar();
        return;
    }

    int encontrados = 0;

    tabela_sem_plano_header();

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            tabela_sem_plano_row(&lista_alunos[i]);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        ui_text_line("Todos os alunos estao vinculados a algum plano.");
    }

    ui_line('-');
    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de alunos sem plano: %d", encontrados);
    ui_text_line(linha);
    ui_text_line("Sugestao: revisar o cadastro e vincular cada aluno a um plano apropriado.");
    ui_line('-');
    aguardar_voltar();
}

static char selecionarFiltroStatus(void)
{
    while (1)
    {
        cabecalho_relatorio("Filtro - Status dos alunos");
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
    }
}

static void solicitarTermoBusca(char *dest, size_t tamanho)
{
    cabecalho_relatorio("Filtro - Termo de busca");
    ui_text_line("Digite parte do nome, ID ou plano para buscar (ENTER para ignorar):");
    ui_line('=');
    printf(">>> ");
    fflush(stdout);
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
        cabecalho_relatorio("Filtro - Ordenacao");
        ui_menu_option('1', "Nome (A-Z)");
        ui_menu_option('2', "ID");
        ui_menu_option('3', "Plano");
        ui_menu_option('4', "Nome (Z-A)");
        ui_menu_option('5', "Idade (menor para maior)");
        ui_menu_option('6', "Idade (maior para menor)");
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
        ui_text_line("Nao foi possivel criar o arquivo CSV.");
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
    char nomeCurto[64];
    ui_clip_utf8(nomeArquivo, 40, nomeCurto, sizeof(nomeCurto));
    ui_text_line("Arquivo gerado com sucesso:");
    ui_text_line(nomeCurto);
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
