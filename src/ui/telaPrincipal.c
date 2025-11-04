#include <stdio.h>
#include "limparTela.h"

char telaPrincipal(void)
{
    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                              SIG-GYM                              ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  ALUNOS                                                      ===\n");
    printf("===  [2]  PLANOS                                                      ===\n");
    printf("===  [3]  EQUIPAMENTOS                                                ===\n");
    printf("===  [4]  FUNCIONARIOS                                                ===\n");
    printf("===  [5]  SOBRE                                                       ===\n");
    printf("===  [6]  BUSCAR POR CPF                                              ===\n"); // 🧭 nova opção adicionada
    printf("===  [0]  SAIR                                                        ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
}
