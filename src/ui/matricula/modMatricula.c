#include <stdio.h>
#include "header_tela.h"

char moduloMatricula(void) {

    char opMatricula;

    do 
    {
        opMatricula = telaMatricula();
        switch(opMatricula) 
        {    
        case '1':       telaCadastrarMatricula();
                        break;
                        
        case '2':       telaVisualizarMatricula();
                        break;
            
        case 
        '3':            telaAtualizarMatricula();
                        break;
            
        case 
        '4':            telaExcluirMatricula();
                        break;
        }
    
    } 
    while (opMatricula != '0');
 
    return opMatricula;
}