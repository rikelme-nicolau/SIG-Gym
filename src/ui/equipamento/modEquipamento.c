#include <stdio.h>

#include "telaEquipamento.h"
#include "cadastrarEquipamento.h"
#include "visualizarEquipamento.h"
#include "atualizarEquipamento.h"
#include "excluirEquipamento.h"

#include "opInvalida.h"

char moduloEquipamento(void)
{
    char opEquipamento;

    do
    {
        opEquipamento = telaEquipamento(); // Função que mostra o menu de equipamentos
        switch (opEquipamento)
        {
        case '1':
            telaCadastrarEquipamento();
            break;

        case '2':
            telaVisualizarEquipamento();
            break;

        case '3':
            telaAtualizarEquipamento();
            break;

        case '4':
            telaExcluirEquipamento();
            break;

        case '0':
            break;

        default:
            opInvalida();
            break;
        }
    } while (opEquipamento != '0');

    return opEquipamento;
}
