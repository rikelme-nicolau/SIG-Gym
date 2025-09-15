#include <stdio.h>
#include "limparTela.h"


char telaExcluirAluno(void) {

    char op;    

    printf("\n");
    printf("====================\n");
    printf("EXCLUIR ALUNO\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    limparTela();
    return op;
}
