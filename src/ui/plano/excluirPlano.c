#include <stdio.h>

char telaExcluirPlano(void) {

    char op;    

    printf("====================\n");
    printf("EXCLUIR PLANO\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
}