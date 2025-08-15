///////////////////////////////////////////////////////////////////////////////
///             Universidade Federal do Rio Grande do Norte                 ///
///                 Centro de Ensino Superior do Seridó                     ///
///               Departamento de Computação e Tecnologia                   ///
///                  Disciplina DCT1106 -- Programação                      ///
///                  Projeto Sistema de Gestão Escolar                      ///
///                  Developed by  - 2025.2                                 ///
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "telaPrincipal.c"

int main(void) {
    char opcao;

    do {
        opcao = telaPrincipal();
    } while (opcao != '0');

    return 0;
}