#include <stdio.h>

#include "telaRelatorio.h"
#include "listagemDados.h"
#include "opInvalida.h"

char moduloRelatorio(void)
{
    char op;

    do
    {
        op = telaRelatorio();

        switch (op)
        {
        case '1':
            relatorioListagemDados();
            break;

        case '2':
            relatorioListagemFiltrada();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }

    } while (op != '0');

    return op;
}
