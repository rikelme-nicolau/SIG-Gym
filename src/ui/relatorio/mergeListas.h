#ifndef MERGE_LISTAS_H
#define MERGE_LISTAS_H

#include "src/ui/aluno/cadastrarAluno.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"

/* Estruturas/assinatura para mesclar as listas de alunos e funcionarios
   em uma unica ordenada por nome. */
struct PessoaView
{
    char tipo; // 'A' aluno, 'F' funcionario
    char id[16];
    char nome[100];
    int idade;
    void *registro_original;
};

struct PessoaView *mergeListasOrdenadas(const struct aluno *alunos, int totalAlunos,
                                        const struct funcionario *funcs, int totalFuncs,
                                        int *outTotal);

#endif // MERGE_LISTAS_H
