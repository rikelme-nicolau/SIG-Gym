#include <stdio.h>

#include "cores.h"

char telaAluno(void) {

char op;

    printf("\n");
    
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                     ALUNO                    ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. CADASTRAR                                 ║\n");
    printf("║ 2. VISUALIZAR                                ║\n");
    printf("║ 3. ATUALIZAR                                 ║\n");
    printf("║ 4. EXCLUIR                                   ║\n");
    printf("║ 0. SAIR                                      ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                   "SUBLINHADO"!ATENÇÃO!"RESET"                  ║\n");
    printf("║  A seguir digite [APENAS] o número da ação:  ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n");

    scanf("%c", &op);
    getchar();
    return op;
    }
