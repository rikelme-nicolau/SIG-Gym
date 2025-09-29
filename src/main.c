#include <stdio.h>

#include "animacao.h"
#include "telaPrincipal.h"
#include "modAluno.h"
#include "modPlano.h"
#include "modFuncionario.h"
#include "modEquipamento.h"
#include "telaSobre.h"
#include "telaFinalizacao.h"
#include "opInvalida.h"
#include "limparTela.h"
#include "src\ui\aluno\arquivoAluno.h"  // persistência alunos
#include "src\ui\plano\arquivoPlano.h"  // persistência plano

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


    animacao();

    total_alunos = carregarAlunos(lista_alunos);
    total_planos = carregarPlanos(lista_planos);

    do {
        op = telaPrincipal();
        switch(op) {
            
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

            moduloPlano();
            limparTela();
            break;

        case '3':

            moduloEquipamento();
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