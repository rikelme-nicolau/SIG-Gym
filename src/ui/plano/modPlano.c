#include <stdio.h>

#include "telaPlano.h"
#include "cadastrarPlano.h"
#include "visualizarPlano.h"
#include "atualizarPlano.h"
#include "excluirPlano.h"

#include "opInvalida.h"

#include "limparTela.h"

char moduloPlano(void) {

    char opPlano;

    do 
    {
        opPlano = telaPlano();
        switch(opPlano) 
        {    
        case '1':       telaCadastrarPlano();
                        limparTela();
                        break;
                        
        case '2':       telaVisualizarPlano();
                        limparTela();
                        break;
            
        case 
        '3':            telaAtualizarPlano();
                        limparTela();
                        break;
            
        case 
        '4':            telaExcluirPlano();
                        limparTela();
                        break;
                        
        case '0':
                        break;

        default:
                        opInvalida();
                        limparTela();
                        break;
        }
    
    } 
    while (opPlano != '0');
 
    return opPlano;
}