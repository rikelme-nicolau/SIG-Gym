#include <stdio.h>

#include "telaMatricula.h"
#include "cadastrarMatricula.h"
#include "visualizarMatricula.h"
#include "atualizarMatricula.h"
#include "excluirMatricula.h"

#include "opInvalida.h"


char moduloMatricula(void) {

    char opMatricula;

    do {
        opMatricula = telaMatricula();
        switch(opMatricula) {    
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
                        
        case '0':
                        break;

        default:
                        opInvalida();
                        break;

        }
    
    } 
    while (opMatricula != '0');
 
    return opMatricula;
}