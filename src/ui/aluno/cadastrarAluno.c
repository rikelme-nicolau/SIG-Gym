#include <stdio.h>
#include "header_tela.h"

char telaCadastrarAluno() {
    
    char op;

    printf("\n");
    printf("=================\n");
    printf(" CADASTRAR ALUNO\n");
    printf("=================\n");
    printf("Nome:\n");
    printf("\n");
    getchar();
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Data de nascimento:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Telefone:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Endereço:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("Email:\n");
    printf("\n");
    printf("<pressione enter para simular!>\n");
    getchar();
    printf("\n");
    printf("Aluno cadastrado com sucesso!\n");
    printf("\n");
    printf("<Ou pressione 0 para retornar!>\n");

    

    scanf(" %c", &op);

    return op;
    }
