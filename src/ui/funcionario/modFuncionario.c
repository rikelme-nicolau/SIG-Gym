#include <stdio.h>

#include "telaFuncionario.h"
#include "cadastrarFuncionario.h"
#include "visualizarFuncionario.h"
#include "atualizarFuncionario.h"
#include "excluirFuncionario.h"

#include "opInvalida.h"

#include "limparTela.h"

char moduloFuncionario(void)
{

    char opCheckin;

    do
    {
        opCheckin = telaFuncionario();
        switch (opCheckin)
        {
        case '1':
            telaCadastrarFuncionario();
            limparTela();
            break;

        case '2':
            telaVisualizarFuncionario();
            limparTela();
            break;

        case '3':
            telaAtualizarFuncionario();
            limparTela();
            break;

        case '4':
            telaExcluirFuncionario();
            limparTela();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (opCheckin != '0');

    return opCheckin;
}