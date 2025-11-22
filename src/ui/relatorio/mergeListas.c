#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "mergeListas.h"
#include "src/ui/utils/validarNascimento.h"

/*
 * Merge de duas listas ordenadas por nome (crescente), uma de alunos e outra de funcionarios.
 * Retorna um array alocado dinamicamente com o resultado e escreve em outTotal o tamanho final.
 * O chamador é responsável por liberar o array retornado via free().
 */
struct PessoaView *mergeListasOrdenadas(const struct aluno *alunos, int totalAlunos,
                                        const struct funcionario *funcs, int totalFuncs,
                                        int *outTotal)
{
    if (outTotal != NULL)
    {
        *outTotal = 0;
    }

    int tamanho = totalAlunos + totalFuncs;
    if (tamanho == 0)
    {
        return NULL;
    }

    struct PessoaView *resultado = malloc(sizeof(struct PessoaView) * tamanho);
    if (resultado == NULL)
    {
        return NULL;
    }

    int i = 0;
    int j = 0;
    int k = 0;

    while (i < totalAlunos && j < totalFuncs)
    {
        const char *nomeAluno = alunos[i].nome;
        const char *nomeFunc = funcs[j].nome;

        if (strcmp(nomeAluno, nomeFunc) <= 0)
        {
            resultado[k].tipo = 'A';
            strncpy(resultado[k].id, alunos[i].id, sizeof(resultado[k].id));
            resultado[k].id[sizeof(resultado[k].id) - 1] = '\0';
            strncpy(resultado[k].nome, alunos[i].nome, sizeof(resultado[k].nome));
            resultado[k].nome[sizeof(resultado[k].nome) - 1] = '\0';
            resultado[k].idade = calcularIdade(alunos[i].idade);
            resultado[k].registro_original = (void *)&alunos[i];
            i++;
        }
        else
        {
            resultado[k].tipo = 'F';
            strncpy(resultado[k].id, funcs[j].id, sizeof(resultado[k].id));
            resultado[k].id[sizeof(resultado[k].id) - 1] = '\0';
            strncpy(resultado[k].nome, funcs[j].nome, sizeof(resultado[k].nome));
            resultado[k].nome[sizeof(resultado[k].nome) - 1] = '\0';
            resultado[k].idade = funcs[j].idade;
            resultado[k].registro_original = (void *)&funcs[j];
            j++;
        }
        k++;
    }

    while (i < totalAlunos)
    {
        resultado[k].tipo = 'A';
        strncpy(resultado[k].id, alunos[i].id, sizeof(resultado[k].id));
        resultado[k].id[sizeof(resultado[k].id) - 1] = '\0';
        strncpy(resultado[k].nome, alunos[i].nome, sizeof(resultado[k].nome));
        resultado[k].nome[sizeof(resultado[k].nome) - 1] = '\0';
        resultado[k].idade = calcularIdade(alunos[i].idade);
        resultado[k].registro_original = (void *)&alunos[i];
        i++;
        k++;
    }

    while (j < totalFuncs)
    {
        resultado[k].tipo = 'F';
        strncpy(resultado[k].id, funcs[j].id, sizeof(resultado[k].id));
        resultado[k].id[sizeof(resultado[k].id) - 1] = '\0';
        strncpy(resultado[k].nome, funcs[j].nome, sizeof(resultado[k].nome));
        resultado[k].nome[sizeof(resultado[k].nome) - 1] = '\0';
        resultado[k].idade = funcs[j].idade;
        resultado[k].registro_original = (void *)&funcs[j];
        j++;
        k++;
    }

    if (outTotal != NULL)
    {
        *outTotal = k;
    }

    return resultado;
}
