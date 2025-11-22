#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "buscaBinaria.h"

static bool verificarOrdenacaoIdAluno(const struct aluno_view *lista, int total)
{
    if (lista == NULL || total <= 1)
    {
        return true;
    }

    for (int i = 1; i < total; i++)
    {
        if (strcmp(lista[i - 1].aluno->id, lista[i].aluno->id) > 0)
        {
            return false;
        }
    }
    return true;
}

/*
 * Busca binaria em lista previamente ordenada por ID (crescente).
 * A cada iteracao, reduz o intervalo de busca pela metade, comparando o ID alvo
 * com o ID do elemento no meio. Retorna o indice se encontrar ou -1 caso contrario.
 * Antes de buscar, valida se a lista esta ordenada.
 */
int buscaBinariaAlunoPorId(const struct aluno_view *lista, int total, const char *id)
{
    if (lista == NULL || id == NULL || total <= 0)
    {
        return -1;
    }

    assert(verificarOrdenacaoIdAluno(lista, total) && "Lista de alunos deve estar ordenada por ID para busca binaria");
    if (!verificarOrdenacaoIdAluno(lista, total))
    {
        printf("Aviso: lista de alunos nao esta ordenada por ID. Ordene antes de usar busca binaria.\n");
        return -1;
    }

    int inicio = 0;
    int fim = total - 1;
    while (inicio <= fim)
    {
        int meio = inicio + (fim - inicio) / 2;
        int cmp = strcmp(lista[meio].aluno->id, id);
        if (cmp == 0)
        {
            return meio;
        }
        else if (cmp < 0)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return -1;
}

static bool verificarOrdenacaoIdFuncionario(const struct funcionario_view *lista, int total)
{
    if (lista == NULL || total <= 1)
    {
        return true;
    }

    for (int i = 1; i < total; i++)
    {
        if (strcmp(lista[i - 1].funcionario->id, lista[i].funcionario->id) > 0)
        {
            return false;
        }
    }
    return true;
}

/*
 * Busca binaria em lista de funcionarios ordenada por ID (crescente).
 * Mesmo algoritmo: compara o ID alvo com o ID do meio para descartar metade do intervalo.
 * Antes de buscar, valida se a lista esta ordenada.
 */
int buscaBinariaFuncionarioPorId(const struct funcionario_view *lista, int total, const char *id)
{
    if (lista == NULL || id == NULL || total <= 0)
    {
        return -1;
    }

    assert(verificarOrdenacaoIdFuncionario(lista, total) && "Lista de funcionarios deve estar ordenada por ID para busca binaria");
    if (!verificarOrdenacaoIdFuncionario(lista, total))
    {
        printf("Aviso: lista de funcionarios nao esta ordenada por ID. Ordene antes de usar busca binaria.\n");
        return -1;
    }

    int inicio = 0;
    int fim = total - 1;
    while (inicio <= fim)
    {
        int meio = inicio + (fim - inicio) / 2;
        int cmp = strcmp(lista[meio].funcionario->id, id);
        if (cmp == 0)
        {
            return meio;
        }
        else if (cmp < 0)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return -1;
}
