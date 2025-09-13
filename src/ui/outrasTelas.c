#include <stdio.h>
#include "outrasTelas.h"

char telaPrincipal(void) {

    char op;

    printf("===================\n");
    printf("SIG-GYM\n");
    printf("===================\n");
    printf("(1) ALUNOS\n");
    printf("(2) PLANOS\n");
    printf("(3) MATRICULAS\n");
    printf("(4) CHECK-INS\n");
    printf("(5) SOBRE\n");
    printf("(0) SAIR\n");

    scanf("%c", &op);
    getchar();

    return op;
    
}



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

}



void telaSobre(void) {
    
    printf("==================\n");
    printf("SOBRE SIG-GYM\n");
    printf("==================\n");
    printf("O sistema, quando finalizado, deve\n");
    printf("apresentar funcioalidades básicas que\n");
    printf("permitam o gerenciamento simples de\n");
    printf("uma academia.\n");
    printf("==================\n");
    printf("Desenvolvido por Rikelme Nicolau & Flávia Juliana\n");
   
    getchar();
}




