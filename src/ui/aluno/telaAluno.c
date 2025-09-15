#include <stdio.h>
#include "limparTela.h"

char telaAluno(void) {

char op;

    printf("\n");
    
    printf("====================\n");
    printf("ALUNO\n");
    printf("====================\n");
    printf("(1) CADASTRAR\n");
    printf("(2) VISUALIZAR\n");
    printf("(3) ATUALIZAR\n");
    printf("(4) EXCLUIR\n");
    printf("(0) SAIR\n");
    scanf("%c", &op);
    getchar();
    
    limparTela();
    return op;
    }
