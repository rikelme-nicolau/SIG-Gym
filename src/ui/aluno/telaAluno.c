#include <stdio.h>
#include "limparTela.h"
#include "atualizarAluno.h"

char telaAluno(void) {
    char op;

    printf("\n");
    printf("=========================================================================\n");
    printf("===                              ALUNO                                ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===  [1]  CADASTRAR                                                   ===\n");
    printf("===  [2]  VISUALIZAR                                                  ===\n");
    printf("===  [3]  ATUALIZAR                                                   ===\n");
    printf("===  [4]  EXCLUIR                                                     ===\n");
    printf("===  [0]  SAIR                                                        ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf(">>> Digite sua opção: "); // Adicionado para clareza

    // 1. O espaço antes de %c ignora quaisquer 'enter' ou 'espaços' anteriores.
    scanf(" %c", &op);

    // 2. A função robusta limpa QUALQUER COISA que o usuário tenha digitado a mais.
    limparBufferEntrada(); 

    limparTela();
    return op;
}