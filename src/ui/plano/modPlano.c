#include <stdio.h>
#include "header_tela.h"

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