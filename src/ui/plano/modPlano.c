#include <stdio.h>

#include "telaPlano.h"
#include "cadastrarPlano.h"
#include "visualizarPlano.h"
#include "atualizarPlano.h"
#include "excluirPlano.h"

#include "opInvalida.h"

char moduloPlano(void) {

    char opPlano;

    do 
    {
        opPlano = telaPlano();
        switch(opPlano) 
        {    
        case '1':       telaCadastrarPlano();
                        break;
                        
        case '2':       telaVisualizarPlano();
                        break;
            
        case 
        '3':            telaAtualizarPlano();
                        break;
            
        case 
        '4':            telaExcluirPlano();
                        break;
                        
        case '0':
                        break;

        default:
                        opInvalida();
                        break;
        }
    
    } 
    while (opPlano != '0');
 
    return opPlano;
}