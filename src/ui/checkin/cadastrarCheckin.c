#include <stdio.h>
#include "limparTela.h"

char telaCadastrarCheckin(void) {
    
    char op;
    
    printf("\n");
    
    printf("=========================================================================\n");
    printf("===                        CADASTRAR CHECKIN                          ===\n");
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
