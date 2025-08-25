#include <stdio.h>
#include "header_tela.h"

char telaExcluirPlano(void) {

    char op;    

    printf("\n");
    printf("=================\n");
    printf(" EXCLUIR PLANO\n");
    printf("=================\n");
    printf("Ids Planos:\n");
    printf("001\n");
    printf("002\n");
    printf("003\n");
    printf("004\n");
    printf("\n");
    printf("Digite o id do plano que deseja excluir:\n");
    printf("\n");
    getchar();
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf(" %c", &op);

    return op;
}