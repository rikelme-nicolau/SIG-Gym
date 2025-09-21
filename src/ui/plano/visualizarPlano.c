#include <stdio.h>
#include "limparTela.h"

char telaVisualizarPlano(void) {

    char op;
    
    printf("====================\n");
    printf("VISUALIZAR PLANO\n");
    printf("====================\n");
    printf("<Aperte 0 para sair!>\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
    }