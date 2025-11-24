#include <stdlib.h>

#include "utilsRelatorios.h"

/* Algoritmos de ordenacao usados pelos relatorios (insertion sort e wrapper
   que decide entre insertion e qsort conforme o tamanho da lista). */

/*
 * Insertion sort: eficiente para listas pequenas.
 * Percorre do segundo elemento ao fim e insere cada item na posição correta
 * entre os já ordenados à esquerda.
 */
/* Lista ordenada em memoria (aluno_view) via insertion sort. */
void insertionSortAlunos(struct aluno_view *lista, int total, int (*compar)(const void *, const void *))
{
    if (lista == NULL || total <= 1 || compar == NULL)
    {
        return;
    }

    for (int i = 1; i < total; i++)
    {
        struct aluno_view chave = lista[i];
        int j = i - 1;
        while (j >= 0 && compar(&lista[j], &chave) > 0)
        {
            lista[j + 1] = lista[j];
            j--;
        }
        lista[j + 1] = chave;
    }
}

/*
 * Escolhe automaticamente o melhor algoritmo:
 * - Listas muito pequenas (<10): insertion sort é simples e tende a ser mais rápido.
 * - Demais casos: delega ao qsort padrão.
 */
/* Lista ordenada em memoria (aluno_view) escolhendo sort dinamicamente. */
void ordenarAlunosOtimizado(struct aluno_view *lista, int total, int (*compar)(const void *, const void *))
{
    if (lista == NULL || total <= 1 || compar == NULL)
    {
        return;
    }

    if (total < 10)
    {
        insertionSortAlunos(lista, total, compar);
    }
    else
    {
        qsort(lista, total, sizeof(struct aluno_view), compar);
    }
}
