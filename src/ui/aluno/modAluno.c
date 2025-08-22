#include <stdio.h>
#include "C:\Users\conta\Documents\vs_sig_gym\SIG-Gym\src\header_tela.h"

void moduloAluno(char op) {


    do {
        switch(op) {
            case '1':   telaCadastrarAluno();
                        break;
            case '2':   telaVisualizarAluno();
                        break;
            case '3':   telaAtualizarAluno();
                        break;
            case '4':   telaExcluirAluno();
                        break;

        }
    } while (op != '0');
}


