#define _XOPEN_SOURCE 700

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

#include "limparTela.h"
#include "opInvalida.h"
#include "relatoriosOperacionais.h"
#include "ui/utils/lerTecla.h"

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/utils/validarTelefone.h"

static void listarManutencoesPorFiltro(char filtro);
static double obterDiasRestantes(const char *data, time_t agora, int *valida);
static const char *descricaoFiltroManutencao(char filtro);
static const struct plano *buscarPlanoOperacionalPorId(const char *id);
static int faixaPrincipalPlano(const struct plano *plano);
static bool planoDisponivelNaFaixa(const struct plano *plano, int faixa);
static int extrairHoraPlano(const char *horario);
static bool campoVazio(const char *texto);
static void adicionarCampo(const char *nomeCampo, char *destino, size_t tamanho);
static int listarAlunosSemPlano(bool exibir);
static int listarAlunosDadosIncompletos(bool exibir);
static int listarFuncionariosDadosIncompletos(bool exibir);
static int listarDuplicadosCPFAlunos(bool exibir);
static int listarDuplicadosCPFFuncionarios(bool exibir);
static int listarEmailsDuplicados(bool exibir);
static int listarTelefonesInvalidos(bool exibir);
static int listarEquipamentosManutencaoUrgente(bool exibir);
static int listarEquipamentosManutencaoAtencao(bool exibir);
static int listarAlunosInativosRecentes(bool exibir);
static int consolidarCadastrosInconsistentes(bool exibir);

void moduloRelatoriosOperacionais(void)
{
    char op;

    do
    {
        printf("\n");
        printf("=================================================================\n");
        printf("===              RELATORIOS OPERACIONAIS - MENU               ===\n");
        printf("=================================================================\n");
        printf("===                                                           ===\n");
        printf("===  [1]  MANUTENCOES DE EQUIPAMENTOS                         ===\n");
        printf("===  [2]  OCUPACAO POR HORARIO                                ===\n");
        printf("===  [3]  VALIDACAO DE CADASTROS                              ===\n");
        printf("===  [4]  ALERTAS E PENDENCIAS                                ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("===                                                           ===\n");
        printf("=================================================================\n");

        op = lerTecla();

        limparTela();

        switch (op)
        {
        case '1':
            relatorioManutencoesEquipamentos();
            break;

        case '2':
            relatorioOcupacaoPorHorario();
            break;

        case '3':
            relatorioValidacaoCadastros();
            break;

        case '4':
            relatorioAlertasPendencias();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (op != '0');
}

void relatorioManutencoesEquipamentos(void)
{
    char op;

    do
    {
        printf("=================================================================\n");
        printf("===        MANUTENCOES DE EQUIPAMENTOS - FILTROS              ===\n");
        printf("=================================================================\n");
        printf("===  [1]  MANUTENCOES VENCIDAS                                ===\n");
        printf("===  [2]  PROXIMAS 7 DIAS                                     ===\n");
        printf("===  [3]  PROXIMAS 15 DIAS                                    ===\n");
        printf("===  [4]  PROXIMAS 30 DIAS                                    ===\n");
        printf("===  [5]  TODAS AS MANUTENCOES                                ===\n");
        printf("===                                                           ===\n");
        printf("===  [0]  VOLTAR                                              ===\n");
        printf("=================================================================\n");

        op = lerTecla();
        limparTela();

        switch (op)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            listarManutencoesPorFiltro(op);
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');
}

void relatorioOcupacaoPorHorario(void)
{
    limparTela();

    int alunosFaixa[3] = {0};
    int totalAlunosConsiderados = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (!lista_alunos[i].ativo)
        {
            continue;
        }

        const struct plano *plano = buscarPlanoOperacionalPorId(lista_alunos[i].plano_id);
        if (plano == NULL)
        {
            continue;
        }

        totalAlunosConsiderados++;

        int faixa = faixaPrincipalPlano(plano);
        if (faixa >= 0 && faixa < 3)
        {
            alunosFaixa[faixa]++;
        }
    }

    printf("=========================================================================\n");
    printf("===               RELATORIO - OCUPACAO POR HORARIO                   ===\n");
    printf("=========================================================================\n");

    if (totalAlunosConsiderados == 0)
    {
        printf("Nao ha alunos ativos vinculados a planos com horarios definidos.\n");
        printf("=========================================================================\n");
        printf(">>> Pressione <ENTER>");
        getchar();
        limparTela();
        return;
    }

    const char *labelsFaixa[3] = {"Manha (06h-12h)", "Tarde (12h-18h)", "Noite (18h-22h)"};

    printf("Total de alunos considerados: %d\n", totalAlunosConsiderados);
    printf("+----------------------+--------+---------------+\n");
    printf("| Faixa                | Alunos | %% Ocupacao   |\n");
    printf("+----------------------+--------+---------------+\n");
    for (int i = 0; i < 3; i++)
    {
        double percentual = totalAlunosConsiderados > 0 ? ((double)alunosFaixa[i] / totalAlunosConsiderados) * 100.0 : 0.0;
        printf("| %-20.20s | %6d | %11.2f %% |\n",
               labelsFaixa[i],
               alunosFaixa[i],
               percentual);
    }
    printf("+----------------------+--------+---------------+\n");

    printf("\nPlanos disponiveis por faixa:\n");
    for (int faixa = 0; faixa < 3; faixa++)
    {
        printf("- %s: ", labelsFaixa[faixa]);
        bool possuiPlano = false;
        for (int i = 0; i < total_planos; i++)
        {
            if (!lista_planos[i].ativo)
            {
                continue;
            }

            if (planoDisponivelNaFaixa(&lista_planos[i], faixa))
            {
                if (possuiPlano)
                {
                    printf(", ");
                }
                printf("%s", lista_planos[i].nome);
                possuiPlano = true;
            }
        }

        if (!possuiPlano)
        {
            printf("Nenhum plano disponivel");
        }
        printf("\n");
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioValidacaoCadastros(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                 RELATORIO - VALIDACAO DE CADASTROS               ===\n");
    printf("=========================================================================\n");

    int totalProblemas = 0;
    totalProblemas += listarAlunosSemPlano(true);
    totalProblemas += listarAlunosDadosIncompletos(true);
    totalProblemas += listarFuncionariosDadosIncompletos(true);
    totalProblemas += listarDuplicadosCPFAlunos(true);
    totalProblemas += listarDuplicadosCPFFuncionarios(true);
    totalProblemas += listarEmailsDuplicados(true);
    totalProblemas += listarTelefonesInvalidos(true);

    if (totalProblemas == 0)
    {
        printf("\nNenhuma inconsistência encontrada nos cadastros analisados.\n");
    }

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

void relatorioAlertasPendencias(void)
{
    limparTela();

    int urgentes = listarEquipamentosManutencaoUrgente(false);
    int atencaoEquip = listarEquipamentosManutencaoAtencao(false);
    int atencaoInativos = listarAlunosInativosRecentes(false);
    int informativos = consolidarCadastrosInconsistentes(false);

    int totalAtencao = atencaoEquip + atencaoInativos;

    printf("=========================================================================\n");
    printf("===                 RELATORIO - ALERTAS E PENDENCIAS                 ===\n");
    printf("=========================================================================\n");
    printf("Resumo: [!!!] %d urgentes | [!!] %d de atencao | [!] %d informativos\n",
           urgentes,
           totalAtencao,
           informativos);
    printf("-------------------------------------------------------------------------\n");

    printf("[!!!] Equipamentos com manutencao vencida (%d)\n", urgentes);
    listarEquipamentosManutencaoUrgente(true);

    printf("\n[!!] Equipamentos com manutencao nos proximos 7 dias (%d)\n", atencaoEquip);
    listarEquipamentosManutencaoAtencao(true);

    printf("\n[!!] Alunos inativos nos ultimos 30 dias (%d)\n", atencaoInativos);
    listarAlunosInativosRecentes(true);

    printf("\n[!] Cadastros com dados inconsistentes (%d)\n", informativos);
    consolidarCadastrosInconsistentes(true);

    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static void listarManutencoesPorFiltro(char filtro)
{
    time_t agora = time(NULL);
    const char *titulo = descricaoFiltroManutencao(filtro);
    int registros = 0;

    printf("=========================================================================\n");
    printf("=== %s\n", titulo);
    printf("=========================================================================\n");
    printf("+--------+------------------------------+----------------------+----------------------+----------------------+-----------------+\n");
    printf("| ID     | Nome                         | Categoria            | Ultima Manutencao    | Proxima Manutencao   | Dias Restantes  |\n");
    printf("+--------+------------------------------+----------------------+----------------------+----------------------+-----------------+\n");

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (!lista_equipamentos[i].ativo)
        {
            continue;
        }

        int valida = 0;
        double diasRestantes = obterDiasRestantes(lista_equipamentos[i].proxima_manutencao, agora, &valida);
        if (!valida)
        {
            continue;
        }

        int exibir = 0;
        switch (filtro)
        {
        case '1':
            exibir = diasRestantes < 0.0;
            break;
        case '2':
            exibir = diasRestantes >= 0.0 && diasRestantes <= 7.0;
            break;
        case '3':
            exibir = diasRestantes >= 0.0 && diasRestantes <= 15.0;
            break;
        case '4':
            exibir = diasRestantes >= 0.0 && diasRestantes <= 30.0;
            break;
        case '5':
            exibir = 1;
            break;
        default:
            exibir = 0;
            break;
        }

        if (!exibir)
        {
            continue;
        }

        const char *cor = "";
        const char *reset = "";
        if (diasRestantes < 0.0)
        {
            cor = ANSI_RED;
            reset = ANSI_RESET;
        }
        else if (diasRestantes < 7.0)
        {
            cor = ANSI_YELLOW;
            reset = ANSI_RESET;
        }

        printf("%s| %-8.8s | %-30.30s | %-22.22s | %-20.20s | %-20.20s | %15.0f |%s\n",
               cor,
               lista_equipamentos[i].id,
               lista_equipamentos[i].nome,
               lista_equipamentos[i].categoria,
               lista_equipamentos[i].ultima_manutencao,
               lista_equipamentos[i].proxima_manutencao,
               diasRestantes,
               reset);

        registros++;
    }

    if (registros == 0)
    {
        printf("| %-121s |\n", "Nenhum equipamento atende ao filtro selecionado.");
    }

    printf("+--------+------------------------------+----------------------+----------------------+----------------------+-----------------+\n");
    printf("Total listado: %d\n", registros);
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER>");
    getchar();
    limparTela();
}

static double obterDiasRestantes(const char *data, time_t agora, int *valida)
{
    struct tm tm_data = {0};
    if (data == NULL || strptime(data, "%d/%m/%Y", &tm_data) == NULL)
    {
        *valida = 0;
        return 0.0;
    }

    time_t alvo = mktime(&tm_data);
    if (alvo == (time_t)-1)
    {
        *valida = 0;
        return 0.0;
    }

    *valida = 1;
    return difftime(alvo, agora) / (60 * 60 * 24);
}

static const char *descricaoFiltroManutencao(char filtro)
{
    switch (filtro)
    {
    case '1':
        return "MANUTENCOES VENCIDAS";
    case '2':
        return "MANUTENCOES PARA OS PROXIMOS 7 DIAS";
    case '3':
        return "MANUTENCOES PARA OS PROXIMOS 15 DIAS";
    case '4':
        return "MANUTENCOES PARA OS PROXIMOS 30 DIAS";
    case '5':
    default:
        return "TODAS AS MANUTENCOES";
    }
}

static const struct plano *buscarPlanoOperacionalPorId(const char *id)
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

static int faixaPrincipalPlano(const struct plano *plano)
{
    int inicio = extrairHoraPlano(plano->horario_inicio);

    if (inicio >= 6 && inicio < 12)
    {
        return 0;
    }
    if (inicio >= 12 && inicio < 18)
    {
        return 1;
    }
    if (inicio >= 18 && inicio < 22)
    {
        return 2;
    }

    return -1;
}

static bool planoDisponivelNaFaixa(const struct plano *plano, int faixa)
{
    static const int faixaInicio[3] = {6, 12, 18};
    static const int faixaFim[3] = {12, 18, 22};

    if (faixa < 0 || faixa >= 3)
    {
        return false;
    }

    int inicio = extrairHoraPlano(plano->horario_inicio);
    int fim = extrairHoraPlano(plano->horario_fim);

    if (inicio < 0 || fim < 0)
    {
        return false;
    }

    if (fim <= inicio)
    {
        fim = inicio + 1;
    }

    return (inicio < faixaFim[faixa]) && (fim > faixaInicio[faixa]);
}

static int extrairHoraPlano(const char *horario)
{
    if (horario == NULL || strlen(horario) < 2 || !isdigit((unsigned char)horario[0]) || !isdigit((unsigned char)horario[1]))
    {
        return -1;
    }

    int hora = (horario[0] - '0') * 10 + (horario[1] - '0');

    if (hora < 0 || hora > 23)
    {
        return -1;
    }

    return hora;
}

static bool campoVazio(const char *texto)
{
    return texto == NULL || texto[0] == '\0';
}

static void adicionarCampo(const char *nomeCampo, char *destino, size_t tamanho)
{
    size_t atual = strlen(destino);
    if (atual >= tamanho - 1)
    {
        return;
    }

    if (destino[0] != '\0')
    {
        size_t disponivel = tamanho - atual - 1;
        strncat(destino, ", ", disponivel);
        atual = strlen(destino);
        if (atual >= tamanho - 1)
        {
            return;
        }
    }

    size_t disponivel = tamanho - strlen(destino) - 1;
    strncat(destino, nomeCampo, disponivel);
}

static int listarAlunosSemPlano(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- Alunos sem plano definido --------------------------------------------\n");
    }
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            if (exibir)
            {
                printf("   - [%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhum aluno com plano pendente.\n");
    }
    return encontrados;
}

static int listarAlunosDadosIncompletos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- Alunos com dados obrigatorios faltando --------------------------------\n");
    }
    for (int i = 0; i < total_alunos; i++)
    {
        char faltantes[128] = "";
        if (campoVazio(lista_alunos[i].nome))
        {
            adicionarCampo("Nome", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_alunos[i].cpf))
        {
            adicionarCampo("CPF", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_alunos[i].telefone))
        {
            adicionarCampo("Telefone", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_alunos[i].email))
        {
            adicionarCampo("Email", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_alunos[i].endereco))
        {
            adicionarCampo("Endereco", faltantes, sizeof(faltantes));
        }

        if (faltantes[0] != '\0')
        {
            if (exibir)
            {
                printf("   - [%s] %s -> Campos: %s\n", lista_alunos[i].id, lista_alunos[i].nome, faltantes);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhum registro com dados faltando.\n");
    }
    return encontrados;
}

static int listarFuncionariosDadosIncompletos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- Funcionarios com dados incompletos ------------------------------------\n");
    }
    for (int i = 0; i < total_funcionarios; i++)
    {
        char faltantes[128] = "";
        if (campoVazio(lista_funcionarios[i].nome))
        {
            adicionarCampo("Nome", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_funcionarios[i].cpf))
        {
            adicionarCampo("CPF", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_funcionarios[i].telefone))
        {
            adicionarCampo("Telefone", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_funcionarios[i].email))
        {
            adicionarCampo("Email", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_funcionarios[i].cargo))
        {
            adicionarCampo("Cargo", faltantes, sizeof(faltantes));
        }
        if (campoVazio(lista_funcionarios[i].endereco))
        {
            adicionarCampo("Endereco", faltantes, sizeof(faltantes));
        }

        if (faltantes[0] != '\0')
        {
            if (exibir)
            {
                printf("   - [%s] %s -> Campos: %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome, faltantes);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhum funcionario com pendencias de dados.\n");
    }
    return encontrados;
}

static int listarDuplicadosCPFAlunos(bool exibir)
{
    bool duplicado[MAX_ALUNOS] = {false};
    for (int i = 0; i < total_alunos; i++)
    {
        if (campoVazio(lista_alunos[i].cpf))
        {
            continue;
        }
        for (int j = i + 1; j < total_alunos; j++)
        {
            if (campoVazio(lista_alunos[j].cpf))
            {
                continue;
            }
            if (strcmp(lista_alunos[i].cpf, lista_alunos[j].cpf) == 0)
            {
                duplicado[i] = true;
                duplicado[j] = true;
            }
        }
    }

    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- CPFs duplicados entre alunos ------------------------------------------\n");
    }
    for (int i = 0; i < total_alunos; i++)
    {
        if (duplicado[i])
        {
            if (exibir)
            {
                printf("   - [%s] %s | CPF: %s\n", lista_alunos[i].id, lista_alunos[i].nome, lista_alunos[i].cpf);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhuma duplicidade encontrada.\n");
    }
    return encontrados;
}

static int listarDuplicadosCPFFuncionarios(bool exibir)
{
    bool duplicado[MAX_FUNCIONARIOS] = {false};
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (campoVazio(lista_funcionarios[i].cpf))
        {
            continue;
        }
        for (int j = i + 1; j < total_funcionarios; j++)
        {
            if (campoVazio(lista_funcionarios[j].cpf))
            {
                continue;
            }
            if (strcmp(lista_funcionarios[i].cpf, lista_funcionarios[j].cpf) == 0)
            {
                duplicado[i] = true;
                duplicado[j] = true;
            }
        }
    }

    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- CPFs duplicados entre funcionarios ------------------------------------\n");
    }
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (duplicado[i])
        {
            if (exibir)
            {
                printf("   - [%s] %s | CPF: %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome, lista_funcionarios[i].cpf);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhuma duplicidade encontrada.\n");
    }
    return encontrados;
}

struct RegistroEmail
{
    const char *tipo;
    const char *id;
    const char *nome;
    const char *email;
};

static int listarEmailsDuplicados(bool exibir)
{
    struct RegistroEmail registros[MAX_ALUNOS + MAX_FUNCIONARIOS];
    bool duplicado[MAX_ALUNOS + MAX_FUNCIONARIOS];
    int totalRegistros = 0;

    for (int i = 0; i < total_alunos && totalRegistros < (int)(sizeof(registros) / sizeof(registros[0])); i++)
    {
        if (campoVazio(lista_alunos[i].email))
        {
            continue;
        }
        registros[totalRegistros].tipo = "Aluno";
        registros[totalRegistros].id = lista_alunos[i].id;
        registros[totalRegistros].nome = lista_alunos[i].nome;
        registros[totalRegistros].email = lista_alunos[i].email;
        duplicado[totalRegistros] = false;
        totalRegistros++;
    }

    for (int i = 0; i < total_funcionarios && totalRegistros < (int)(sizeof(registros) / sizeof(registros[0])); i++)
    {
        if (campoVazio(lista_funcionarios[i].email))
        {
            continue;
        }
        registros[totalRegistros].tipo = "Funcionario";
        registros[totalRegistros].id = lista_funcionarios[i].id;
        registros[totalRegistros].nome = lista_funcionarios[i].nome;
        registros[totalRegistros].email = lista_funcionarios[i].email;
        duplicado[totalRegistros] = false;
        totalRegistros++;
    }

    for (int i = 0; i < totalRegistros; i++)
    {
        for (int j = i + 1; j < totalRegistros; j++)
        {
            if (strcmp(registros[i].email, registros[j].email) == 0)
            {
                duplicado[i] = true;
                duplicado[j] = true;
            }
        }
    }

    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- Emails duplicados -----------------------------------------------------\n");
    }
    for (int i = 0; i < totalRegistros; i++)
    {
        if (duplicado[i])
        {
            if (exibir)
            {
                printf("   - (%s) [%s] %s | Email: %s\n", registros[i].tipo, registros[i].id, registros[i].nome, registros[i].email);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        printf("   Nenhuma duplicidade de email encontrada.\n");
    }
    return encontrados;
}

static int listarTelefonesInvalidos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        printf("\n-- Telefones invalidos ---------------------------------------------------\n");
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (campoVazio(lista_alunos[i].telefone))
        {
            continue;
        }
        if (!validarTelefone(lista_alunos[i].telefone))
        {
            if (exibir)
            {
                printf("   - (Aluno) [%s] %s | Telefone: %s\n", lista_alunos[i].id, lista_alunos[i].nome, lista_alunos[i].telefone);
            }
            encontrados++;
        }
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (campoVazio(lista_funcionarios[i].telefone))
        {
            continue;
        }
        if (!validarTelefone(lista_funcionarios[i].telefone))
        {
            if (exibir)
            {
                printf("   - (Funcionario) [%s] %s | Telefone: %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome, lista_funcionarios[i].telefone);
            }
            encontrados++;
        }
    }

    if (encontrados == 0 && exibir)
    {
        printf("   Todos os telefones preenchidos passaram na validacao.\n");
    }
    return encontrados;
}

static int listarEquipamentosManutencaoUrgente(bool exibir)
{
    time_t agora = time(NULL);
    int encontrados = 0;

    if (exibir)
    {
        printf("   Tabela de equipamentos com manutencao vencida:\n");
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
        printf("   | ID     | Nome                         | Categoria            | Ultima               | Proxima              | Dias Rest.    |\n");
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
    }

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (!lista_equipamentos[i].ativo)
        {
            continue;
        }
        int valida = 0;
        double dias = obterDiasRestantes(lista_equipamentos[i].proxima_manutencao, agora, &valida);
        if (!valida || dias >= 0.0)
        {
            continue;
        }
        encontrados++;
        if (exibir)
        {
            printf("   | %-8.8s | %-30.30s | %-20.20s | %-20.20s | %-20.20s | %11.0f |\n",
                   lista_equipamentos[i].id,
                   lista_equipamentos[i].nome,
                   lista_equipamentos[i].categoria,
                   lista_equipamentos[i].ultima_manutencao,
                   lista_equipamentos[i].proxima_manutencao,
                   dias);
        }
    }

    if (exibir)
    {
        if (encontrados == 0)
        {
            printf("   | %-121s |\n", "Nenhum equipamento consta com manutencao vencida.");
        }
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
    }

    return encontrados;
}

static int listarEquipamentosManutencaoAtencao(bool exibir)
{
    time_t agora = time(NULL);
    int encontrados = 0;

    if (exibir)
    {
        printf("   Tabela de equipamentos com manutencao nos proximos 7 dias:\n");
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
        printf("   | ID     | Nome                         | Categoria            | Ultima               | Proxima              | Dias Rest.    |\n");
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
    }

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (!lista_equipamentos[i].ativo)
        {
            continue;
        }
        int valida = 0;
        double dias = obterDiasRestantes(lista_equipamentos[i].proxima_manutencao, agora, &valida);
        if (!valida || dias < 0.0 || dias > 7.0)
        {
            continue;
        }
        encontrados++;
        if (exibir)
        {
            printf("   | %-8.8s | %-30.30s | %-20.20s | %-20.20s | %-20.20s | %11.0f |\n",
                   lista_equipamentos[i].id,
                   lista_equipamentos[i].nome,
                   lista_equipamentos[i].categoria,
                   lista_equipamentos[i].ultima_manutencao,
                   lista_equipamentos[i].proxima_manutencao,
                   dias);
        }
    }

    if (exibir)
    {
        if (encontrados == 0)
        {
            printf("   | %-121s |\n", "Nenhum equipamento previsto para manutencao nos proximos 7 dias.");
        }
        printf("   +--------+------------------------------+----------------------+----------------------+----------------------+---------------+\n");
    }

    return encontrados;
}

static int listarAlunosInativosRecentes(bool exibir)
{
    if (exibir)
    {
        printf("   Informacao indisponivel: nao ha registro da data de inativacao dos alunos.\n");
    }
    return 0;
}

static int consolidarCadastrosInconsistentes(bool exibir)
{
    int total = 0;
    total += listarAlunosSemPlano(exibir);
    total += listarAlunosDadosIncompletos(exibir);
    total += listarFuncionariosDadosIncompletos(exibir);
    total += listarDuplicadosCPFAlunos(exibir);
    total += listarDuplicadosCPFFuncionarios(exibir);
    total += listarEmailsDuplicados(exibir);
    total += listarTelefonesInvalidos(exibir);
    return total;
}
