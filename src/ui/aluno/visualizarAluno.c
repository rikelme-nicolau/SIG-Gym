#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaVisualizarAluno(void) {

    char op;
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║               VISUALIZAR ALUNO                ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║  Listados abaixo estão os alunos atualmente   ║\n");
    printf("║             cadastrados e ativos:             ║\n");
    printf("╠══════╦════════════════════════════════════════╣\n");
    printf("║  IDs:║ NOMES:                                 ║\n");//posteriormente add função que realiza esses prints de forma automática com nomes realmente cadastrados
    printf("║  001 ║ Enzo Primeiro                          ║\n");
    printf("║  002 ║ Valentina Primeira                     ║\n");
    printf("║  003 ║ Enzo Segundo                           ║\n");
    printf("║  004 ║ Valentina Segunda                      ║\n");
    printf("╠══════╩════════════════════════════════════════╣\n");
    printf("║                   "SUBLINHADO"!ATENÇÃO!"RESET"                   ║\n");
    printf("║    A seguir digite [APENAS] o ID do aluno:    ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\n");
    printf("<operação de visualização ainda não está pronta, pressione 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;
    }
