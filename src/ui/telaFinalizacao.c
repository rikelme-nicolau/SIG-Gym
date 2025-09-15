#include <stdio.h>
#include "limparTela.h"

void telaFinalizacao(void) {
    printf("\n");
    printf("               FIM DO PROGRAMA                \n");
    printf("      Obrigado por utilizar o SIG-Gym!        \n");
    printf("      O programa foi desenvolvido para fins     \n");
    printf("      de aprendizado e não fins comerciais.     \n");
    printf("===================================================\n");
    printf("      DCT1106 -- Programação.             \n");
    printf("      Ministrada pelo professor e orientador    \n");
    printf("      Flavius Gorgônio.              \n");
    printf("      Curso - Bacharelado em Sistemas da Informação \n");
    printf("      Centro de Ensino Superior do Seridó      \n");
    printf("      Departamento de Computação e Tecnologia    \n");
    printf("===================================================\n");
    printf("      <Pressione <ENTER> para finalizar>       \n");
    
    printf("\n");
    getchar();

    limparTela();
}