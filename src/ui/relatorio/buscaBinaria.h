#ifndef BUSCA_BINARIA_H
#define BUSCA_BINARIA_H

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"

/* Views auxiliares para permitir busca binaria em listas ordenadas
   de alunos e funcionarios. */
struct aluno_view
{
    const struct aluno *aluno;
    const struct plano *plano;
};

struct funcionario_view
{
    const struct funcionario *funcionario;
};

int buscaBinariaAlunoPorId(const struct aluno_view *lista, int total, const char *id);
int buscaBinariaFuncionarioPorId(const struct funcionario_view *lista, int total, const char *id);

#endif // BUSCA_BINARIA_H
