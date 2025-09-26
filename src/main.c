#include <stdio.h>

#include "animacao.h"
#include "telaPrincipal.h"
#include "modAluno.h"
#include "modPlano.h"
#include "modFuncionario.h"
#include "telaSobre.h"
#include "telaFinalizacao.h"
#include "opInvalida.h"
#include "limparTela.h"

int main(void)
{
    char op;

    animacao();

    do
    {
        op = telaPrincipal();
        switch (op)
        {

        case '1':
            moduloAluno();
            limparTela();
            break;

        case '2':

            moduloPlano();
            limparTela();
            break;

        case '3':

            limparTela();
            break;

        case '4':

            moduloFuncionario();
            limparTela();
            break;

        case '5':
            telaSobre();
            limparTela();
            break;
        case '0':
            telaFinalizacao();
            limparTela();
            break;

        default:
            opInvalida();
            limparTela();
            break;
        }
    } while (op != '0');

    return 0;
}