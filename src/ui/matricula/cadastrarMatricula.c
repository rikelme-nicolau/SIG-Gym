#include <stdio.h>
#include "limparTela.h"

char telaCadastrarMatricula(void){
    
    char op;

    printf("====================\n");
    printf("CADASTRAR MATRICULA\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
    }
