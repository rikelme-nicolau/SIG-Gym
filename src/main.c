#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "animacao.h"
#include "telaPrincipal.h"
#include "modAluno.h"
#include "modPlano.h"
#include "modFuncionario.h"
#include "modEquipamento.h"
#include "modRelatorio.h"
#include "telaSobre.h"
#include "telaFinalizacao.h"
#include "opInvalida.h"
#include "limparTela.h"
#include "src/ui/utils/logGeracao.h"
#include "src/ui/utils/resetarDados.h"

// Persistência
#include "src/ui/aluno/arquivoAluno.h"             // lista_alunos[], total_alunos
#include "src/ui/plano/arquivoPlano.h"             // lista_planos[], total_planos
#include "src/ui/funcionario/arquivoFuncionario.h" // lista_funcionarios[], total_funcionarios
#include "src/ui/equipamento/arquivoEquipamento.h" // lista_equipamentos[], total_equipamentos

int main(void)
{

    char op;

    srand((unsigned int)time(NULL));

    // animação inicial
    animacao();

    // carrega dados persistentes
    total_alunos = carregarAlunos(lista_alunos);
    total_planos = carregarPlanos(lista_planos);
    total_funcionarios = carregarFuncionarios(lista_funcionarios);
    total_equipamentos = carregarEquipamentos(lista_equipamentos);

    logFimGeracao();

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
            moduloEquipamento();
            limparTela();
            break;

        case '4':
            moduloFuncionario();
            limparTela();
            break;

        case '5':
            moduloRelatorio();
            limparTela();
            break;

        case '6':
            telaSobre();
            limparTela();
            break;

        case 'R':
        case 'r':
        {
            printf("ATENCAO: Isso apagara TODOS os dados! Confirma? (S/N): ");
            int resp = getchar();
            int flush;
            while ((flush = getchar()) != '\n' && flush != EOF)
                ;

            if (resp == 'S' || resp == 's')
            {
                limparTela();
                regerarDadosFicticios();
            }
            else
            {
                printf("Operacao cancelada.\n");
            }
            printf("Pressione <ENTER> para continuar...");
            getchar();
            limparTela();
            break;
        }

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
