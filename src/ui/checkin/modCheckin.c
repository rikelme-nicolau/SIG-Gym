#include <stdio.h>

#include "telaCheckin.h"
#include "cadastrarCheckin.h"
#include "visualizarCheckin.h"
#include "atualizarCheckin.h"
#include "excluirCheckin.h"

#include "opInvalida.h"

char moduloCheckin(void) {

    char opCheckin;

    do {
        opCheckin = telaCheckin();
        switch(opCheckin) {    
        case '1':       telaCadastrarCheckin();
                        break;
                        
        case '2':       telaVisualizarCheckin();
                        break;
            
        case 
        '3':            telaAtualizarCheckin();
                        break;
            
        case 
        '4':            telaExcluirCheckin();
                        break;

        case '0':
                        break;
                        
        default:
                        opInvalida();
                        break;
        }
    
    } 
    while (opCheckin != '0');
 
    return opCheckin;
}