#include <stdio.h>

char telaExcluirPlano(void) {

    char op;    

    printf("====================\n");
    printf("EXCLUIR PLANO\n");
    printf("====================\n");
    scanf("%c", &op);
    getchar();

    return op;
}