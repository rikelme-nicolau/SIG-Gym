#include <stdio.h>
#include "header_tela.h"

char telaCadastrarPlano(void){
    
    char op;

    printf("\n");
    printf("=================\n");
    printf(" CADASTRAR PLANO\n");
    printf("=================\n");

    printf("Nome do plano:\n");
    printf("\n");
    getchar();
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Duração do plano:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Valor do plano:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("\n");
    printf("<Ou pressione 0 para retornar!>\n");


    scanf(" %c", &op);

    return op;
    }