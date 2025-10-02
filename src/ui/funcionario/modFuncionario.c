#include <stdio.h>

#include "telaFuncionario.h"
#include "cadastrarFuncionario.h"
#include "visualizarFuncionario.h"
#include "atualizarFuncionario.h"
#include "excluirFuncionario.h"

#include "opInvalida.h"

char moduloFuncionario(void)
{
    char opFunc;

    do
    {
        opFunc = telaFuncionario();
        switch (opFunc)
        {
        case '1':
            telaCadastrarFuncionario();
            break;

        case '2':
            telaVisualizarFuncionario();
            break;

        case '3':
            telaAtualizarFuncionario();
            break;

        case '4':
            telaExcluirFuncionario();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (opFunc != '0');

    return opFunc;
}
