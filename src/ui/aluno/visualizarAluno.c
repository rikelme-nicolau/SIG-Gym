#include <stdio.h>
#include "header_tela.h"

char telaVisualizarAluno(void) {

    char op;
    printf("\n");
    printf("=================\n");
    printf(" VISUALIZAR ALUNO\n");
    printf("=================\n");
    printf("Ids:\n");
    printf("001\n");
    printf("002\n");
    printf("003\n");
    printf("004\n");
    printf("\n");
    printf("Digite o id do aluno que deseja visualizar:\n");
    printf("\n");
    getchar();
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    
    scanf(" %c", &op);

    return op;
    }
