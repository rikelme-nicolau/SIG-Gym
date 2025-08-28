#include <stdio.h>
#include "header_tela.h"
#include "cores.h"
char telaCadastrarCheckin(void) {
    
    char op;

    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║               CADASTRAR CHECKIN               ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    }
