#include <stdio.h>
#include "limparTela.h"

char telaVisualizarCheckin(void) {

    char op;
    printf("\n");
   
    printf("=========================================================================\n");
    printf("===                        VISUALIZAR CHECKIN                         ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]                                                              ===\n");
    printf("===  [2]                                                              ===\n");
    printf("===  [3]                                                              ===\n");
    printf("===  [4]                                                              ===\n");
    printf("===  [5]                                                              ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
    }
