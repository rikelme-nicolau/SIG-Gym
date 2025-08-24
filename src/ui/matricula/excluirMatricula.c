#include <stdio.h>
#include "header_tela.h"

char telaExcluirMatricula(void) {

    char op;    

    printf("\n");
    printf("=================\n");
    printf(" EXCLUIR MATRICULA\n");
    printf("=================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf(" %c", &op);

    return op;
}
