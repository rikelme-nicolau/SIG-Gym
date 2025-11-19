#include <stdio.h>
#include <string.h>

#include "relatoriosCruzados.h"
#include "telaRelatoriosCruzados.h"
#include "opInvalida.h"
#include "limparTela.h"

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"

static void relatorioRelacaoAlunoPlano(void);
static const struct plano *buscarPlanoPorId(const char *id);

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

    printf("============================================================\n");
    printf("                  RELAÇÃO ALUNO / PLANO                     \n");
    printf("============================================================\n");
    printf("ID   NOME DO ALUNO            PLANO                 VALOR   \n");
    printf("------------------------------------------------------------\n");

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

        printf("%-4.4s %-24.24s %-21.21s %10.2f\n",
               lista_alunos[i].id,
               lista_alunos[i].nome,
               nome_plano,
               valor_plano);
        total_relacoes++;
    }

    if (total_relacoes == 0)
    {
        printf("Nenhuma relação aluno/plano encontrada.\n");
    }

    printf("------------------------------------------------------------\n");
    printf("Total de registros: %d\n", total_relacoes);
    printf("============================================================\n");
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
