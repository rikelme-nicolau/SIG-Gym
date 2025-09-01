#include <stdio.h>

#include "animacao.h"
#include "telaPrincipal.h"
#include "modAluno.h"
#include "modPlano.h"
#include "modMatricula.h"
#include "modCheckin.h"
#include "telaSobre.h"
#include "telaFinalizacao.h"
#include "opInvalida.h"





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