#include <stdio.h>

char moduloPlanos(void) {

    printf("=================================\n");
    printf("|         MÓDULO PLANOS         |\n");
    printf("=================================\n");
    printf("| 1. Cadastrar Novo Plano       |\n");
    printf("| 2. Atualizar Plano            |\n");
    printf("| 3. Visualizar Planos          |\n");
    printf("| 4. Deletar Plano              |\n");
    printf("| 0. Sair                       |\n");
    printf("=================================\n");

    char op_plano;
    printf("O que deseja fazer? Escolha uma das opções acima: ");
    scanf(" %c", &op_plano); 

    return op_plano;
}