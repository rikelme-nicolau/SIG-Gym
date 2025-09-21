#include <stdio.h>

#include "telaCheckin.h"
#include "cadastrarCheckin.h"
#include "visualizarCheckin.h"
#include "atualizarCheckin.h"
#include "excluirCheckin.h"

#include "opInvalida.h"

#include "limparTela.h"

char moduloCheckin(void) {

    char opCheckin;

    do {
        opCheckin = telaCheckin();
        switch(opCheckin) {    
        case '1':       telaCadastrarCheckin();
        limparTela();
                        break;
                        
        case '2':       telaVisualizarCheckin();
        limparTela();
                        break;
            
        case 
        '3':            telaAtualizarCheckin();
        limparTela();
                        break;
            
        case 
        '4':            telaExcluirCheckin();
        limparTela();
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