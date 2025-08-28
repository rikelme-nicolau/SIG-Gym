#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaAtualizarMatricula(void) {

    char op;

    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║              ATUALIZAR MATRICULA              ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;

    }
