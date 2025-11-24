#include <stdio.h>

#include "telaEquipamento.h"
#include "cadastrarEquipamento.h"
#include "visualizarEquipamento.h"
#include "atualizarEquipamento.h"
#include "excluirEquipamento.h"

#include "opInvalida.h"

/* Controla o fluxo do modulo de equipamentos: exibe o menu, chama cada tela
   de cadastro, listagem, edicao e exclusao ate o usuario optar por voltar. */
char moduloEquipamento(void)
{
    char opEquip;

    do
    {
        opEquip = telaEquipamento();
        switch (opEquip)
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
    } while (opEquip != '0');

    return opEquip;
}
