#include <stdio.h>

#include "cores.h"

char telaExcluirPlano(void) {

    char op;    

    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║                 EXCLUIR PLANO                 ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║  Listados abaixo estão os planos atualmente   ║\n");
    printf("║             cadastrados e ativos:             ║\n");
    printf("╠══════╦════════════════════════════════════════╣\n");
    printf("║  IDs:║ PLANOS:                                 ║\n");//posteriormente add função que realiza esses prints de forma automática com nomes realmente cadastrados
    printf("║  001 ║ Semanal                                ║\n");
    printf("║  002 ║ Mensal                                 ║\n");
    printf("║  003 ║ Anual                                  ║\n");
    printf("║  004 ║ Diário                                 ║\n");
    printf("╠══════╩════════════════════════════════════════╣\n");
    printf("║                   "SUBLINHADO"!ATENÇÃO!"RESET"                   ║\n");
    printf("║    A seguir digite [APENAS] o ID do plano:    ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\n");
    printf("<operação de exclusão ainda não está pronta, pressione 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
}