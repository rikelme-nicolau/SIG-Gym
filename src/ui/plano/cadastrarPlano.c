#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"

#define MAX_PLANOS 50
#define MAX_BUFFER 1024
#define MAX_ATIVIDADES 5

struct plano {
    char id[12];
    char nome[MAX_BUFFER];
    char horario[MAX_BUFFER];
    char atividades[MAX_ATIVIDADES][MAX_BUFFER];
    int total_atividades;
    bool ativo;
};

struct plano lista_planos[MAX_PLANOS];
int total_planos = 0;

void telaCadastrarPlano(void) {
    if(total_planos >= MAX_PLANOS) {
        printf("Limite de planos atingido!\n");
        getchar();
        return;
    }

    struct plano novo_plano;
    char buffer[MAX_BUFFER];

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>>digite o nome do plano: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_plano.nome, buffer);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>>digite o horario de funcionamento: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_plano.horario, buffer);

    novo_plano.total_atividades = 0;
    for(int i = 0; i < MAX_ATIVIDADES; i++) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR PLANO                            ===\n");
        printf("=========================================================================\n");
        printf(">>>digite a atividade #%d (ou <ENTER> para finalizar): ", i+1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if(strlen(buffer) == 0) break; 
        strcpy(novo_plano.atividades[i], buffer);
        novo_plano.total_atividades++;
    }

    snprintf(novo_plano.id, sizeof(novo_plano.id), "%d", total_planos + 1);
    novo_plano.ativo = true;

    lista_planos[total_planos++] = novo_plano;

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>>press <ENTER>");
    getchar();
    limparTela();
}
