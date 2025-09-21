#include <stdio.h>

#include "telaMatricula.h"
#include "cadastrarMatricula.h"
#include "visualizarMatricula.h"
#include "atualizarMatricula.h"
#include "excluirMatricula.h"

#include "opInvalida.h"

#include "limparTela.h"

char moduloMatricula(void) {

    char opMatricula;

    do {
        opMatricula = telaMatricula();
        switch(opMatricula) {    
        case '1':       telaCadastrarMatricula();
                        limparTela();
                        break;
                        
        case '2':       telaVisualizarMatricula();
                        limparTela();
                        break;
            
        case 
        '3':            telaAtualizarMatricula();
                        limparTela();
                        break;
            
        case 
        '4':            telaExcluirMatricula();
                        limparTela();
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