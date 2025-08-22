
#include <stdio.h>
#include "C:\Users\conta\Documents\vs_sig_gym\SIG-Gym\src\header_tela.h"

void telaAluno(void) {

char op;

    printf("\n");
    
    printf("=================\n");
    printf("    Aluno\n");
    printf("=================\n");
    printf("(1) Cadastrar \n");
    printf("(2) Visualizar\n");
    printf("(3) Atualizar\n");
    printf("(4) Excluir\n");
    printf("\n");

    scanf(" %c", &op);
    modulo_aluno p = moduloAluno;
    p(op);
}
