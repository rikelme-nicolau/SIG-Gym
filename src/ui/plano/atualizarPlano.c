#include <stdio.h>
#include "limparTela.h"

char telaAtualizarPlano(void) {

    char op;

    printf("====================\n");
    printf("ATUALIZAR PLANO\n");
    printf("====================\n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
    }