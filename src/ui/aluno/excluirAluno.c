#include <stdio.h>

#include "cores.h"

char telaExcluirAluno(void) {

    char op;    

    printf("\n");
    printf("====================\n");
    printf("EXCLUIR ALUNO\n");
    printf("====================\n");
    scanf("%c", &op);
    getchar();

    return op;
}
