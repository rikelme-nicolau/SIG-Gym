#include <stdio.h>
#include "ui/telaPrincipal.c"
#include "ui/aluno/telaAluno.c"
#include "ui/plano/telaPlano.c"
#include "ui/checkin/telaCheckin.c"
#include "ui/matricula/telaMatriculas.c"
#include "ui/telaSobre.c"


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