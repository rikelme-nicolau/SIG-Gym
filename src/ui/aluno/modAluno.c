#include <stdio.h>

#include "telaAluno.h"
#include "cadastrarAluno.h"
#include "visualizarAluno.h"
#include "atualizarAluno.h"
#include "excluirAluno.h"

#include "opInvalida.h"

/* Controla o menu principal do modulo de alunos:
   exibe as telas e delega para as operacoes de cadastro, listagem, edicao
   e exclusao enquanto o usuario nao escolher voltar. */
char moduloAluno(void)
{
    char opAluno;

    do
    {
        opAluno = telaAluno();
        switch (opAluno)
        {
        case '1':
            telaCadastrarAluno();
            break;

        case '2':
            telaVisualizarAluno();
            break;

        case '3':
            telaAtualizarAluno();
            break;

        case '4':
            telaExcluirAluno();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (opAluno != '0');

    return opAluno;
}
