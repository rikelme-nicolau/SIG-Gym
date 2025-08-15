///////////////////////////////////////////////////////////////////////////////
///             Universidade Federal do Rio Grande do Norte                 ///
///                 Centro de Ensino Superior do Seridó                     ///
///               Departamento de Computação e Tecnologia                   ///
///                  Disciplina DCT1106 -- Programação                      ///
///                  Projeto Sistema de Gestão Escolar                      ///
///                  Developed by  - 2025.2                                 ///
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

int main(void) {
    char opcao;

    do {
        opcao = telaPrincipal();
        switch(opcao) {
            case '1':   moduloAluno();
                break;
            case '2':   moduloPlanos();
                break;
            case '3':   moduloMatricula();
                break;
            case '4':   moduloFluxo();
                break;
            case '5':   moduloRH();
                break;
            case '6':   telaSobre();
                break;
        }
    } while (opcao != '0');

    return 0;
}