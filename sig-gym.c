///////////////////////////////////////////////////////////////////////////////
///             Universidade Federal do Rio Grande do Norte                 ///
///                 Centro de Ensino Superior do Seridó                     ///
///               Departamento de Computação e Tecnologia                   ///
///                  Disciplina DCT1106 -- Programação                      ///
///                  Projeto Sistema de Gestão Escolar                      ///
///         Developed by Rikelme Nicolau & Flávia Juliana- 2025.2           ///
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "telaPrincipal.c"
#include "moduloMatriculas.c"
#include "telaAluno.c"
#include "telaPlano.c"
#include "telaSobre.c"



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
            case '4':   //moduloCheck-ins();
                        break;
            case '5':   telaSobre();
                        break;
        }
    } while (op != '0');

    return 0;
}