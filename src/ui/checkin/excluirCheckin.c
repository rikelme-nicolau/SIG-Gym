#include <stdio.h>

#include "cores.h"
char telaExcluirCheckin(void) {

    char op;    

    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║                EXCLUIR CHECKIN                ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
}
