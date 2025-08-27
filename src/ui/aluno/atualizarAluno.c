#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaAtualizarAluno(void) {

    char op;
    //char id[5];

    printf("\n");
    printf(RED"_________________________________________________\n"RESET);
    printf(RED"|                                               |\n"RESET);
    printf(RED"|"YELLOW"                ATUALIZAR ALUNO                "RED"|\n"RESET);
    printf(RED"|_______________________________________________|\n"RESET);
    printf(RED"|"YELLOW"  Listados abaixo estão os alunos atualmente   "RED"|\n"RESET);
    printf(RED"|"YELLOW"             cadastrados e ativos:             "RED"|\n"RESET);
    printf(RED"|-----------------------------------------------|\n"RESET);
    printf(RED"|"YELLOW"  IDs:"RED"|"YELLOW" NOMES:                                 "RED"|\n"RESET);//posteriormente add função que realiza esses prints de forma automática com nomes realmente cadastrados
    printf(RED"|"YELLOW"  001 "RED"|"YELLOW" Enzo Primeiro                          "RED"|\n"RESET);
    printf(RED"|"YELLOW"  002 "RED"|"YELLOW" Valentina Primeira                     "RED"|\n"RESET);
    printf(RED"|"YELLOW"  003 "RED"|"YELLOW" Enzo Segundo                           "RED"|\n"RESET);
    printf(RED"|"YELLOW"  004 "RED"|"YELLOW" Valentina Segunda                      "RED"|\n"RESET);
    printf(RED"|-----------------------------------------------|\n"RESET);
    printf(RED"|"YELLOW"                   "SUBLINHADO"!ATENÇÃO!"RESET"                   "RED"|\n"RESET);
    printf(RED"|"YELLOW"    A seguir digite [APENAS] o ID do aluno:    "RED"|\n"RESET);
    printf(RED"|_______________________________________________|\n"RESET);
    printf("\n");
    //fgets(id, 5, stdin); 
    printf("<operação de atualização ainda não está pronta, aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;

    }
