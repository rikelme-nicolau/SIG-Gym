#include <stdio.h>
#include "header_tela.h"


int main(void) {
    char op;

    do {
        op = telaPrincipal();
        switch(op) {
            case '1':   telaAluno();
                        break;
            case '2':   telaPlano();
                        break;
            case '3':   telaMatriculas();
                        break;
            case '4':   telaCheckin();
                        break;
            case '5':   telaSobre();
                        break;
        }
    } while (op != '0');

    return 0;
}