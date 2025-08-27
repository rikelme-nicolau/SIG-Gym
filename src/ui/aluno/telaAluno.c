#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaAluno(void) {

char op;

    printf("\n");
    
    printf(RED"________________________________________________\n"RESET);
    printf(RED"|                                              |\n"RESET);
    printf(RED"|                 "YELLOW"MÓDULO ALUNO                 "RED"|\n"RESET);
    printf(RED"|______________________________________________|\n"RESET);
    printf(RED"|"YELLOW" 1. CADASTRAR                                 "RED"|\n"RESET);
    printf(RED"|"YELLOW" 2. VISUALIZAR                                "RED"|\n"RESET);
    printf(RED"|"YELLOW" 3. ATUALIZAR                                 "RED"|\n"RESET);
    printf(RED"|"YELLOW" 4. EXCLUIR                                   "RED"|\n"RESET);
    printf(RED"|"YELLOW" 0. SAIR                                      "RED"|\n"RESET);
    printf(RED"|----------------------------------------------|\n"RESET);
    printf(RED"|                   "YELLOW SUBLINHADO"!ATENÇÃO!"RESET"                  "RED"|\n"RESET);
    printf(RED"|"YELLOW"  A seguir digite [APENAS] o número da ação:  "RED"|\n"RESET);
    printf(RED"|______________________________________________|\n"RESET);
    printf("\n");

    scanf("%c", &op);
    getchar();
    return op;
    }
