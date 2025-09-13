#include <stdio.h>

#include "animacao.h"
#include "outrasTelas.h"
#include "telasAluno.h"
#include "telasPlano.h"
#include "telasMatricula.h"
#include "telasCheckin.h"
#include "telasErros.h"





int main(void) {
    char op;

    animacao();

    do {
        op = telaPrincipal();
        switch(op) {
            
        case '1':  
                        moduloAluno();
                        break;
            
        case '2': 
            
                        moduloPlano();
                        break;
                                    
        case '3':  
            
                        moduloMatricula();
                        break;

        case '4':
                                    
                        moduloCheckin();                            
                        break;
            
        case '5':
                        telaSobre();
                        break;
        case '0':
                        telaFinalizacao();
                        break;

        default:
                        opInvalida();
                        break;
                
    }
     }while (op != '0');

    return 0;
}