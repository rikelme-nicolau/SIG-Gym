#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaCadastrarPlano(void){
    
    char op;

    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                CADASTRAR PLANO               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║   As informações necessárias para realizar   ║\n");
    printf("║      o cadastro estão listadas a abaixo:     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. NOME DO PLANO                             ║\n");
    printf("║ 2. DURAÇÃO                                   ║\n");
    printf("║ 3. VALOR                                     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n");

    printf("< pressione 0 para retornar!>\n");


    scanf("%c", &op);
    getchar();

    return op;
    }