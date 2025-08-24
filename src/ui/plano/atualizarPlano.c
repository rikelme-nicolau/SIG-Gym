#include <stdio.h>
#include "header_tela.h"

char telaAtualizarPlano(void) {

    char op;

    printf("\n");
    printf("=================\n");
    printf(" ATUALIZAR PLANO\n");
    printf("=================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf(" %c", &op);

    return op;

    }