#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"

char* telaCadastrarAluno(void) {
    
    char buffer[1024];

    printf("\n");
    
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    char *nome = malloc(strlen(buffer) + 1);
    strcpy(nome, buffer);

    limparTela();
    return nome;
    }
