#include <stdio.h>
#include "header_tela.h"
#include "cores.h"

char telaCadastrarAluno() {
    
    char op;

    printf("\n");
    printf(RED"________________________________________________\n"RESET);
    printf(RED"|                                              |\n"RESET);
    printf(RED"|"YELLOW"                CADASTRAR ALUNO               "RED"|\n"RESET);
    printf(RED"|______________________________________________|\n"RESET);
    printf(RED"|"YELLOW"   As informações necessárias para realizar   "RED"|\n"RESET);
    printf(RED"|"YELLOW"      o cadastro estão listadas a abaixo:     "RED"|\n"RESET);
    printf(RED"|----------------------------------------------|\n"RESET);
    printf(RED"|"YELLOW" 1. NOME COMPLETO     "RED"|"YELLOW" 2. DATA DE NASCIMENTO "RED"|\n"RESET);
    printf(RED"|"YELLOW" 3. CPF               "RED"|"YELLOW" 4. ENDEREÇO           "RED"|\n"RESET);
    printf(RED"|"YELLOW" 5. TELEFONE          "RED"|"YELLOW" 6. EMAIL              "RED"|\n"RESET);
    printf(RED"|______________________________________________|\n"RESET);
    printf("\n");
    printf("<operação de cadastro ainda não está pronta, pressione 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    }
