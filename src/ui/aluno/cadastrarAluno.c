#include <stdio.h>

#include "cores.h"

char telaCadastrarAluno() {
    
    char op;

    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                CADASTRAR ALUNO               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║   As informações necessárias para realizar   ║\n");
    printf("║      o cadastro estão listadas a abaixo:     ║\n");
    printf("╠═════════════════════╦════════════════════════╣\n");
    printf("║ 1. NOME COMPLETO    ║ 2. DATA DE NASCIMENTO  ║\n");
    printf("║ 3. CPF              ║ 4. ENDEREÇO            ║\n");
    printf("║ 5. TELEFONE         ║ 6. EMAIL               ║\n");
    printf("╚═════════════════════╩════════════════════════╝\n");
    printf("\n");
    printf("<operação de cadastro ainda não está pronta, pressione 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    }
