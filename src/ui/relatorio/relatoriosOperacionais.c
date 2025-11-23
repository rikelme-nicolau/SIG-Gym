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
#include "ui/utils/consoleLayout.h"

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

static void cabecalho(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

static void linha_tabela(const int *w, int n, const char **vals)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    for (int i = 0; i < n; i++)
    {
        ui_append_col(linha, sizeof(linha), &pos, vals[i], w[i]);
        if (i < n - 1)
        {
            ui_append_sep(linha, sizeof(linha), &pos);
        }
    }
    linha[pos] = '\0';
    ui_text_line(linha);
}

void moduloRelatoriosOperacionais(void)
{
    char op;

    do
    {
        limparTela();
        ui_header("SIG-GYM", "Relatorios Operacionais");
        ui_empty_line();
        ui_menu_option('1', "Manutencoes de equipamentos");
        ui_menu_option('2', "Ocupacao por horario");
        ui_menu_option('3', "Validacao de cadastros");
        ui_menu_option('4', "Alertas e pendencias");
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
        cabecalho("Relatorio - Manutencoes de Equipamentos");
        ui_menu_option('1', "Manutencoes vencidas");
        ui_menu_option('2', "Proximas 7 dias");
        ui_menu_option('3', "Proximas 15 dias");
        ui_menu_option('4', "Proximas 30 dias");
        ui_menu_option('5', "Todas as manutencoes");
        ui_empty_line();
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha um filtro");
        ui_line('=');
        printf(">>> ");
        fflush(stdout);

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
    cabecalho("Relatorio - Ocupacao por Horario");

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

    if (totalAlunosConsiderados == 0)
    {
        ui_text_line("Nao ha alunos ativos vinculados a planos com horarios definidos.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    const char *labelsFaixa[3] = {"Manha (06h-12h)", "Tarde (12h-18h)", "Noite (18h-22h)"};

    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total de alunos considerados: %d", totalAlunosConsiderados);
    ui_text_line(linha);
    const int w[3] = {22, 6, 10};
    const char *cab[3] = {"Faixa", "Alunos", "% Ocupacao"};
    linha_tabela(w, 3, cab);
    for (int i = 0; i < 3; i++)
    {
        double percentual = totalAlunosConsiderados > 0 ? ((double)alunosFaixa[i] / totalAlunosConsiderados) * 100.0 : 0.0;
        char alunosStr[12], percStr[16];
        snprintf(alunosStr, sizeof(alunosStr), "%d", alunosFaixa[i]);
        snprintf(percStr, sizeof(percStr), "%.2f%%", percentual);
        const char *vals[3] = {labelsFaixa[i], alunosStr, percStr};
        linha_tabela(w, 3, vals);
    }

    ui_line('-');
    ui_text_line("Planos disponiveis por faixa:");
    for (int faixa = 0; faixa < 3; faixa++)
    {
        char buffer[UI_INNER + 1] = "";
        strncat(buffer, labelsFaixa[faixa], sizeof(buffer) - 1);
        strncat(buffer, ": ", sizeof(buffer) - strlen(buffer) - 1);
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
                    strncat(buffer, ", ", sizeof(buffer) - strlen(buffer) - 1);
                }
                strncat(buffer, lista_planos[i].nome, sizeof(buffer) - strlen(buffer) - 1);
                possuiPlano = true;
            }
        }

        if (!possuiPlano)
        {
            strncat(buffer, "Nenhum plano disponivel", sizeof(buffer) - strlen(buffer) - 1);
        }
        ui_text_line(buffer);
    }

    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

void relatorioValidacaoCadastros(void)
{
    cabecalho("Relatorio - Validacao de Cadastros");

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
        ui_text_line("Nenhuma inconsistencia encontrada nos cadastros analisados.");
    }

    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

void relatorioAlertasPendencias(void)
{
    cabecalho("Relatorio - Alertas e Pendencias");

    int urgentes = listarEquipamentosManutencaoUrgente(false);
    int atencaoEquip = listarEquipamentosManutencaoAtencao(false);
    int atencaoInativos = listarAlunosInativosRecentes(false);
    int informativos = consolidarCadastrosInconsistentes(false);

    int totalAtencao = atencaoEquip + atencaoInativos;

    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Resumo: [!!!] %d urgentes | [!!] %d de atencao | [!] %d informativos",
             urgentes, totalAtencao, informativos);
    ui_text_line(linha);
    ui_line('-');

    snprintf(linha, sizeof(linha), "[!!!] Equipamentos com manutencao vencida (%d)", urgentes);
    ui_text_line(linha);
    listarEquipamentosManutencaoUrgente(true);

    ui_line('-');
    snprintf(linha, sizeof(linha), "[!!] Equipamentos com manutencao nos proximos 7 dias (%d)", atencaoEquip);
    ui_text_line(linha);
    listarEquipamentosManutencaoAtencao(true);

    ui_line('-');
    snprintf(linha, sizeof(linha), "[!!] Alunos inativos nos ultimos 30 dias (%d)", atencaoInativos);
    ui_text_line(linha);
    listarAlunosInativosRecentes(true);

    ui_line('-');
    snprintf(linha, sizeof(linha), "[!] Cadastros com dados inconsistentes (%d)", informativos);
    ui_text_line(linha);
    consolidarCadastrosInconsistentes(true);

    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

static void listarManutencoesPorFiltro(char filtro)
{
    time_t agora = time(NULL);
    const char *titulo = descricaoFiltroManutencao(filtro);
    int registros = 0;

    cabecalho(titulo);
    const int w[6] = {8, 20, 14, 12, 12, 6};
    const char *cab[6] = {"ID", "Nome", "Categoria", "Ultima", "Proxima", "Dias"};
    linha_tabela(w, 6, cab);

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

        (void)cor;
        (void)reset;

        char nome[64];
        char categoria[64];
        char dias[16];
        ui_clip_utf8(lista_equipamentos[i].nome, w[1], nome, sizeof(nome));
        ui_clip_utf8(lista_equipamentos[i].categoria, w[2], categoria, sizeof(categoria));
        snprintf(dias, sizeof(dias), "%.0f", diasRestantes);
        const char *vals[6] = {
            lista_equipamentos[i].id,
            nome,
            categoria,
            lista_equipamentos[i].ultima_manutencao,
            lista_equipamentos[i].proxima_manutencao,
            dias};
        linha_tabela(w, 6, vals);

        registros++;
    }

    if (registros == 0)
    {
        ui_text_line("Nenhum equipamento atende ao filtro selecionado.");
    }

    char linha[UI_INNER + 1];
    snprintf(linha, sizeof(linha), "Total listado: %d", registros);
    ui_text_line(linha);
    ui_section_title("Pressione <ENTER> para voltar");
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
        ui_text_line("-- Alunos sem plano definido");
    }
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].plano_id[0] == '\0' || strcmp(lista_alunos[i].plano_id, "0") == 0)
        {
            if (exibir)
            {
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - [%s] %s", lista_alunos[i].id, lista_alunos[i].nome);
                ui_text_line(linha);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhum aluno com plano pendente.");
    }
    return encontrados;
}

static int listarAlunosDadosIncompletos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        ui_text_line("-- Alunos com dados obrigatorios faltando");
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
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - [%s] %s -> Campos: %s", lista_alunos[i].id, lista_alunos[i].nome, faltantes);
                ui_text_line(linha);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhum registro com dados faltando.");
    }
    return encontrados;
}

static int listarFuncionariosDadosIncompletos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        ui_text_line("-- Funcionarios com dados incompletos");
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
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - [%s] %s -> Campos: %s", lista_funcionarios[i].id, lista_funcionarios[i].nome, faltantes);
                ui_text_line(linha);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhum funcionario com pendencias de dados.");
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
        ui_text_line("-- CPFs duplicados entre alunos");
    }
    for (int i = 0; i < total_alunos; i++)
    {
            if (duplicado[i])
            {
                if (exibir)
                {
                    char linha[UI_INNER + 1];
                    char nome_buf[64];
                    ui_clip_utf8(lista_alunos[i].nome, 40, nome_buf, sizeof(nome_buf));
                    snprintf(linha, sizeof(linha), "   - [%.12s] %s | CPF: %.14s", lista_alunos[i].id, nome_buf, lista_alunos[i].cpf);
                    ui_text_line(linha);
                }
                encontrados++;
            }
        }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhuma duplicidade encontrada.");
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
        ui_text_line("-- CPFs duplicados entre funcionarios");
    }
    for (int i = 0; i < total_funcionarios; i++)
    {
            if (duplicado[i])
            {
                if (exibir)
                {
                    char linha[UI_INNER + 1];
                    char nome_buf[64];
                    ui_clip_utf8(lista_funcionarios[i].nome, 40, nome_buf, sizeof(nome_buf));
                    snprintf(linha, sizeof(linha), "   - [%.12s] %s | CPF: %.14s", lista_funcionarios[i].id, nome_buf, lista_funcionarios[i].cpf);
                    ui_text_line(linha);
                }
                encontrados++;
            }
        }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhuma duplicidade encontrada.");
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
        ui_text_line("-- Emails duplicados");
    }
    for (int i = 0; i < totalRegistros; i++)
    {
        if (duplicado[i])
        {
            if (exibir)
            {
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - (%s) [%s] %s | Email: %s", registros[i].tipo, registros[i].id, registros[i].nome, registros[i].email);
                ui_text_line(linha);
            }
            encontrados++;
        }
    }
    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Nenhuma duplicidade de email encontrada.");
    }
    return encontrados;
}

static int listarTelefonesInvalidos(bool exibir)
{
    int encontrados = 0;
    if (exibir)
    {
        ui_text_line("-- Telefones invalidos");
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
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - (Aluno) [%s] %s | Telefone: %s", lista_alunos[i].id, lista_alunos[i].nome, lista_alunos[i].telefone);
                ui_text_line(linha);
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
                char linha[UI_INNER + 1];
                snprintf(linha, sizeof(linha), "   - (Funcionario) [%s] %s | Telefone: %s", lista_funcionarios[i].id, lista_funcionarios[i].nome, lista_funcionarios[i].telefone);
                ui_text_line(linha);
            }
            encontrados++;
        }
    }

    if (encontrados == 0 && exibir)
    {
        ui_text_line("   Todos os telefones preenchidos passaram na validacao.");
    }
    return encontrados;
}

static int listarEquipamentosManutencaoUrgente(bool exibir)
{
    time_t agora = time(NULL);
    int encontrados = 0;

    if (exibir)
    {
        const int w[6] = {8, 20, 14, 12, 12, 6};
        const char *cab[6] = {"ID", "Nome", "Categoria", "Ultima", "Proxima", "Dias"};
        ui_text_line("   Equipamentos com manutencao vencida:");
        linha_tabela(w, 6, cab);
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
            const int w[6] = {8, 20, 14, 12, 12, 6};
            char nome[64], categoria[64], diasStr[16];
            ui_clip_utf8(lista_equipamentos[i].nome, 20, nome, sizeof(nome));
            ui_clip_utf8(lista_equipamentos[i].categoria, 14, categoria, sizeof(categoria));
            snprintf(diasStr, sizeof(diasStr), "%.0f", dias);
            const char *vals[6] = {
                lista_equipamentos[i].id,
                nome,
                categoria,
                lista_equipamentos[i].ultima_manutencao,
                lista_equipamentos[i].proxima_manutencao,
                diasStr};
            linha_tabela(w, 6, vals);
        }
    }

    if (exibir)
    {
        if (encontrados == 0)
        {
            ui_text_line("   Nenhum equipamento consta com manutencao vencida.");
        }
    }

    return encontrados;
}

static int listarEquipamentosManutencaoAtencao(bool exibir)
{
    time_t agora = time(NULL);
    int encontrados = 0;

    if (exibir)
    {
        const int w[6] = {8, 20, 14, 12, 12, 6};
        const char *cab[6] = {"ID", "Nome", "Categoria", "Ultima", "Proxima", "Dias"};
        ui_text_line("   Equipamentos com manutencao nos proximos 7 dias:");
        linha_tabela(w, 6, cab);
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
            const int w[6] = {8, 20, 14, 12, 12, 6};
            char nome[64], categoria[64], diasStr[16];
            ui_clip_utf8(lista_equipamentos[i].nome, 20, nome, sizeof(nome));
            ui_clip_utf8(lista_equipamentos[i].categoria, 14, categoria, sizeof(categoria));
            snprintf(diasStr, sizeof(diasStr), "%.0f", dias);
            const char *vals[6] = {
                lista_equipamentos[i].id,
                nome,
                categoria,
                lista_equipamentos[i].ultima_manutencao,
                lista_equipamentos[i].proxima_manutencao,
                diasStr};
            linha_tabela(w, 6, vals);
        }
    }

    if (exibir)
    {
        if (encontrados == 0)
        {
            ui_text_line("   Nenhum equipamento previsto para manutencao nos proximos 7 dias.");
        }
    }

    return encontrados;
}

static int listarAlunosInativosRecentes(bool exibir)
{
    if (exibir)
    {
        ui_text_line("   Informacao indisponivel: nao ha registro da data de inativacao dos alunos.");
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
