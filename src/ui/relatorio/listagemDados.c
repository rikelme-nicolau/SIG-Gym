#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "limparTela.h"
#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/equipamento/cadastrarEquipamento.h"
#include "ui/utils/lerTecla.h"

#define BUFFER_FILTRO 1024

/* Relatorios de listagem simples com filtro por status e termo,
   usados como versoes anteriores/demonstrativas. */

static void toLowerString(const char *src, char *dest, size_t dest_size)
{
    if (dest_size == 0)
    {
        return;
    }

    size_t i = 0;
    for (; src[i] != '\0' && i + 1 < dest_size; ++i)
    {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

static bool textoContem(const char *texto, const char *busca)
{
    if (busca[0] == '\0')
    {
        return true;
    }

    char texto_lower[BUFFER_FILTRO];
    char busca_lower[BUFFER_FILTRO];

    toLowerString(texto, texto_lower, sizeof(texto_lower));
    toLowerString(busca, busca_lower, sizeof(busca_lower));

    return strstr(texto_lower, busca_lower) != NULL;
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

static const char *descricaoStatusFiltro(char filtroStatus)
{
    switch (filtroStatus)
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

static void imprimirCabecalhoFiltro(void)
{
    printf("=========================================================================\n");
    printf("===        RELATÓRIO - LISTAGEM COM APLICAÇÃO DE FILTRO              ===\n");
    printf("=========================================================================\n\n");
}

static const char *descricaoTipoFiltro(char tipo)
{
    switch (tipo)
    {
    case '1':
        return "Alunos";
    case '2':
        return "Planos";
    case '3':
        return "Funcionários";
    case '4':
        return "Equipamentos";
    default:
        return "";
    }
}

static void relatorioFiltradoAlunos(char filtroStatus, const char *termo)
{
    printf("ALUNOS\n");
    printf("--------------------------------------------------------------------------\n");

    if (total_alunos == 0)
    {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    int encontrados = 0;

    for (int i = 0; i < total_alunos; i++)
    {
        if (!statusCorresponde(lista_alunos[i].ativo, filtroStatus))
        {
            continue;
        }

        bool correspondeTermo = textoContem(lista_alunos[i].nome, termo) ||
                                textoContem(lista_alunos[i].id, termo) ||
                                textoContem(lista_alunos[i].plano_id, termo);

        if (!correspondeTermo)
        {
            continue;
        }

        encontrados++;
        printf("  [%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
        printf("      Plano: %s | Status: %s\n",
               lista_alunos[i].plano_id[0] != '\0' ? lista_alunos[i].plano_id : "Não informado",
               lista_alunos[i].ativo ? "Ativo" : "Inativo");
    }

    if (encontrados == 0)
    {
        printf("Nenhum aluno encontrado com os critérios informados.\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------\n");
        printf("Total encontrado: %d\n", encontrados);
    }
}

static void relatorioFiltradoPlanos(char filtroStatus, const char *termo)
{
    printf("PLANOS\n");
    printf("--------------------------------------------------------------------------\n");

    if (total_planos == 0)
    {
        printf("Nenhum plano cadastrado.\n");
        return;
    }

    int encontrados = 0;

    for (int i = 0; i < total_planos; i++)
    {
        if (!statusCorresponde(lista_planos[i].ativo, filtroStatus))
        {
            continue;
        }

        bool correspondeTermo = textoContem(lista_planos[i].nome, termo) ||
                                textoContem(lista_planos[i].id, termo);

        if (!correspondeTermo)
        {
            for (int j = 0; j < lista_planos[i].total_atividades; j++)
            {
                if (textoContem(lista_planos[i].atividades[j], termo))
                {
                    correspondeTermo = true;
                    break;
                }
            }
        }

        if (!correspondeTermo)
        {
            continue;
        }

        encontrados++;
        printf("  [%s] %s\n", lista_planos[i].id, lista_planos[i].nome);
        printf("      Horário: %s - %s | Atividades: %d | Status: %s\n",
               lista_planos[i].horario_inicio,
               lista_planos[i].horario_fim,
               lista_planos[i].total_atividades,
               lista_planos[i].ativo ? "Ativo" : "Inativo");
    }

    if (encontrados == 0)
    {
        printf("Nenhum plano encontrado com os critérios informados.\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------\n");
        printf("Total encontrado: %d\n", encontrados);
    }
}

static void relatorioFiltradoFuncionarios(char filtroStatus, const char *termo)
{
    printf("FUNCIONÁRIOS\n");
    printf("--------------------------------------------------------------------------\n");

    if (total_funcionarios == 0)
    {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }

    int encontrados = 0;

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (!statusCorresponde(lista_funcionarios[i].ativo, filtroStatus))
        {
            continue;
        }

        bool correspondeTermo = textoContem(lista_funcionarios[i].nome, termo) ||
                                textoContem(lista_funcionarios[i].id, termo) ||
                                textoContem(lista_funcionarios[i].cargo, termo) ||
                                textoContem(lista_funcionarios[i].cpf, termo);

        if (!correspondeTermo)
        {
            continue;
        }

        encontrados++;
        printf("  [%s] %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome);
        printf("      Cargo: %s | Status: %s\n",
               lista_funcionarios[i].cargo,
               lista_funcionarios[i].ativo ? "Ativo" : "Inativo");
    }

    if (encontrados == 0)
    {
        printf("Nenhum funcionário encontrado com os critérios informados.\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------\n");
        printf("Total encontrado: %d\n", encontrados);
    }
}

static void relatorioFiltradoEquipamentos(char filtroStatus, const char *termo)
{
    printf("EQUIPAMENTOS\n");
    printf("--------------------------------------------------------------------------\n");

    if (total_equipamentos == 0)
    {
        printf("Nenhum equipamento cadastrado.\n");
        return;
    }

    int encontrados = 0;

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (!statusCorresponde(lista_equipamentos[i].ativo, filtroStatus))
        {
            continue;
        }

        bool correspondeTermo = textoContem(lista_equipamentos[i].nome, termo) ||
                                textoContem(lista_equipamentos[i].id, termo) ||
                                textoContem(lista_equipamentos[i].categoria, termo);

        if (!correspondeTermo)
        {
            continue;
        }

        encontrados++;
        printf("  [%s] %s\n", lista_equipamentos[i].id, lista_equipamentos[i].nome);
        printf("      Categoria: %s | Próx. manutenção: %s | Status: %s\n",
               lista_equipamentos[i].categoria,
               lista_equipamentos[i].proxima_manutencao,
               lista_equipamentos[i].ativo ? "Ativo" : "Inativo");
    }

    if (encontrados == 0)
    {
        printf("Nenhum equipamento encontrado com os critérios informados.\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------\n");
        printf("Total encontrado: %d\n", encontrados);
    }
}

void relatorioListagemDados(void)
{
    limparTela();

    printf("=========================================================================\n");
    printf("===                 RELATÓRIO - LISTAGEM DE DADOS                     ===\n");
    printf("=========================================================================\n\n");

    int alunos_ativos = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            alunos_ativos++;
        }
    }
    printf("ALUNOS ATIVOS: %d\n", alunos_ativos);
    if (alunos_ativos > 0)
    {
        for (int i = 0; i < total_alunos; i++)
        {
            if (lista_alunos[i].ativo)
            {
                printf("  - [%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
            }
        }
    }
    else
    {
        printf("  Nenhum aluno ativo encontrado.\n");
    }

    printf("\n");

    int planos_ativos = 0;
    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            planos_ativos++;
        }
    }
    printf("PLANOS ATIVOS: %d\n", planos_ativos);
    if (planos_ativos > 0)
    {
        for (int i = 0; i < total_planos; i++)
        {
            if (lista_planos[i].ativo)
            {
                printf("  - [%s] %s\n", lista_planos[i].id, lista_planos[i].nome);
            }
        }
    }
    else
    {
        printf("  Nenhum plano ativo encontrado.\n");
    }

    printf("\n");

    int funcionarios_ativos = 0;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            funcionarios_ativos++;
        }
    }
    printf("FUNCIONÁRIOS ATIVOS: %d\n", funcionarios_ativos);
    if (funcionarios_ativos > 0)
    {
        for (int i = 0; i < total_funcionarios; i++)
        {
            if (lista_funcionarios[i].ativo)
            {
                printf("  - [%s] %s (%s)\n", lista_funcionarios[i].id, lista_funcionarios[i].nome, lista_funcionarios[i].cargo);
            }
        }
    }
    else
    {
        printf("  Nenhum funcionário ativo encontrado.\n");
    }

    printf("\n");

    int equipamentos_ativos = 0;
    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            equipamentos_ativos++;
        }
    }
    printf("EQUIPAMENTOS ATIVOS: %d\n", equipamentos_ativos);
    if (equipamentos_ativos > 0)
    {
        for (int i = 0; i < total_equipamentos; i++)
        {
            if (lista_equipamentos[i].ativo)
            {
                printf("  - [%s] %s - Próx. manutenção: %s\n", lista_equipamentos[i].id, lista_equipamentos[i].nome, lista_equipamentos[i].proxima_manutencao);
            }
        }
    }
    else
    {
        printf("  Nenhum equipamento ativo encontrado.\n");
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER> para voltar...");
    getchar();

    limparTela();
}

void relatorioListagemFiltrada(void)
{
    limparTela();

    char tipo_registro;
    while (1)
    {
        imprimirCabecalhoFiltro();
        printf("===  Escolha o tipo de registro que deseja filtrar:                  ===\n");
        printf("===                                                                   ===\n");
        printf("===    [1]  Alunos                                                    ===\n");
        printf("===    [2]  Planos                                                    ===\n");
        printf("===    [3]  Funcionários                                              ===\n");
        printf("===    [4]  Equipamentos                                              ===\n");
        printf("===                                                                   ===\n");
        printf("===    [0]  Voltar                                                    ===\n");
        printf("===                                                                   ===\n");
        printf("=========================================================================\n");
        printf("Opção: ");

        tipo_registro = lerTecla();

        if (tipo_registro == '0' || tipo_registro == '1' || tipo_registro == '2' ||
            tipo_registro == '3' || tipo_registro == '4')
        {
            break;
        }

        printf("\n>>> Opção inválida! Pressione <ENTER> para tentar novamente...");
        getchar();
        limparTela();
    }

    if (tipo_registro == '0')
    {
        limparTela();
        return;
    }

    char filtroStatus;
    while (1)
    {
        limparTela();
        imprimirCabecalhoFiltro();
        printf("===  Tipo selecionado: %-47s===\n", descricaoTipoFiltro(tipo_registro));
        printf("===                                                                   ===\n");
        printf("===  Selecione o status desejado:                                     ===\n");
        printf("===                                                                   ===\n");
        printf("===    [1]  Somente ativos                                            ===\n");
        printf("===    [2]  Somente inativos                                          ===\n");
        printf("===    [3]  Todos                                                     ===\n");
        printf("===                                                                   ===\n");
        printf("=========================================================================\n");
        printf("Opção: ");

        filtroStatus = lerTecla();

        if (filtroStatus == '1' || filtroStatus == '2' || filtroStatus == '3')
        {
            break;
        }

        printf("\n>>> Opção inválida! Pressione <ENTER> para tentar novamente...");
        getchar();
    }

    limparTela();
    imprimirCabecalhoFiltro();
    printf("===  Tipo selecionado: %-47s===\n", descricaoTipoFiltro(tipo_registro));
    printf("===  Status: %-57s===\n", descricaoStatusFiltro(filtroStatus));
    printf("===                                                                   ===\n");
    printf("===  Digite parte do nome, ID ou outra informação para filtrar.       ===\n");
    printf("===  Pressione <ENTER> para ignorar.                                  ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("Filtro: ");

    char termo[BUFFER_FILTRO];
    if (fgets(termo, sizeof(termo), stdin) != NULL)
    {
        termo[strcspn(termo, "\n")] = '\0';
    }
    else
    {
        termo[0] = '\0';
    }

    limparTela();
    imprimirCabecalhoFiltro();
    printf("===  Tipo selecionado: %-47s===\n", descricaoTipoFiltro(tipo_registro));
    printf("===  Status: %-57s===\n", descricaoStatusFiltro(filtroStatus));
    printf("===  Termo: %-58s===\n", termo[0] != '\0' ? termo : "Nenhum");
    printf("=========================================================================\n\n");

    switch (tipo_registro)
    {
    case '1':
        relatorioFiltradoAlunos(filtroStatus, termo);
        break;
    case '2':
        relatorioFiltradoPlanos(filtroStatus, termo);
        break;
    case '3':
        relatorioFiltradoFuncionarios(filtroStatus, termo);
        break;
    case '4':
        relatorioFiltradoEquipamentos(filtroStatus, termo);
        break;
    default:
        break;
    }

    printf("\n=========================================================================\n");
    printf(">>> Pressione <ENTER> para voltar...");
    getchar();

    limparTela();
}
