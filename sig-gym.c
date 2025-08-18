#include <stdio.h>
#include "telaPrincipal.c"
#include "moduloMatriculas.c"
#include "telaAluno.c"
#include "telaPlano.c"
#include "telaSobre.c"
#include "moduloCheckin.c"



int main(void) {
    char op;

    do {
        op = telaPrincipal();
        switch(op) {
            case '1':   moduloAluno();
                        break;
            case '2':   moduloPlanos();
                        break;
            case '3':   moduloMatriculas();
                        break;
            case '4':   moduloCheckin();
                        break;
            case '5':   telaSobre();
                        break;
        }
    } while (op != '0');

    return 0;
}