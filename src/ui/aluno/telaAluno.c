#include <stdio.h>
#include "limparTela.h"
#include "atualizarAluno.h"
#include "ui/utils/lerTecla.h"

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

    op = lerTecla();

    limparTela();
    return op;
}
