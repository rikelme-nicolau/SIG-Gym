#include <stdio.h>
#include "limparTela.h"

char telaPlano(void) {

char op;

    printf("\n");
    
    printf("=========================================================================\n");
    printf("===                              PLANO                                ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  CADASTRAR                                                   ===\n");
    printf("===  [2]  VISUALIZAR                                                  ===\n");
    printf("===  [3]  ATUALIZAR                                                   ===\n");
    printf("===  [4]  EXCLUIR                                                     ===\n");
    printf("===  [0]  SAIR                                                        ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
}