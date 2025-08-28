#include <stdio.h>
#include "cores.h"

char telaPrincipal(void) {

    char op;

    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                    SIG-GYM                   ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. ALUNOS                                    ║\n");
    printf("║ 2. PLANOS                                    ║\n");
    printf("║ 3. MATRICULAS                                ║\n");
    printf("║ 4. CHECK-INS                                 ║\n");
    printf("║ 5. SOBRE                                     ║\n");
    printf("║ 0. SAIR                                      ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                   "SUBLINHADO"!ATENÇÃO!"RESET"                  ║\n");
    printf("║  A seguir digite [APENAS] o número da ação:  ║\n");
    printf("╚══════════════════════════════════════════════╝\n");

    scanf("%c", &op);
    getchar();

    return op;
    }




