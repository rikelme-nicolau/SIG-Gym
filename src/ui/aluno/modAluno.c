#include <stdio.h>
#include "header_tela.h"

char moduloAluno(void) {

    char opAluno;
    
    

    do 
    {
        opAluno = telaAluno();
        switch(opAluno) 
        {    
        case '1':       telaCadastrarAluno();
                        break;
                        
        case
        '2':            telaVisualizarAluno();
                        break;
            
        case 
        '3':            telaAtualizarAluno();
                        break;
            
        case 
        '4':            telaExcluirAluno();
                        break;

        case '0':
                        break;
                        
        default:
                        opInvalida();
                        break;
        }
    } 
    while (opAluno != '0');
 
    return opAluno;
}