#include <stdio.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"

char telaCadastrarFuncionario(void)
{

    char op;

    printf("\n");

    printf("=========================================================================\n");
    printf("===                        CADASTRAR FUNCIONÁRIO                      ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");

    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
}
