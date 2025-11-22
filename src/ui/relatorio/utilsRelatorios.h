#ifndef UTILS_RELATORIOS_H
#define UTILS_RELATORIOS_H

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"

struct aluno_view
{
    const struct aluno *aluno;
    const struct plano *plano;
};

void insertionSortAlunos(struct aluno_view *lista, int total, int (*compar)(const void *, const void *));
void ordenarAlunosOtimizado(struct aluno_view *lista, int total, int (*compar)(const void *, const void *));

#endif // UTILS_RELATORIOS_H
