#include <stdio.h>

#include "cores.h"

char telaVisualizarAluno(void) {

    char op;
    printf("\n");
    printf("====================\n");
    printf("VISUALIZAR ALUNO \n");
    printf("====================\n");
    scanf("%c", &op);
    getchar();

    return op;
    }
