#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"

#include "cadastrarPlano.h"
#include "arquivoPlano.h" // persistência

#define MAX_PLANOS 50
#define MAX_BUFFER 1024
#define MAX_ATIVIDADES 5

struct plano lista_planos[MAX_PLANOS];
int total_planos = 0;

void telaCadastrarPlano(void)
{
    if (total_planos >= MAX_PLANOS)
    {
        printf("Limite de planos atingido!\n");
        getchar();
        return;
    }

    struct plano novo_plano;
    char buffer[MAX_BUFFER];

    // Nome
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>> Digite o nome do plano: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_plano.nome, buffer);

    // Horário início
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>> Digite o horário de INÍCIO (ex: 10:00): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_plano.horario_inicio, buffer);

    // Horário fim

    // Horário inicial

    limparTela();

    // Horário final
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>> Digite o horário de FIM (ex: 20:00): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_plano.horario_fim, buffer);

    // Atividades
    novo_plano.total_atividades = 0;
    for (int i = 0; i < MAX_ATIVIDADES; i++)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR PLANO                            ===\n");
        printf("=========================================================================\n");
        printf(">>> Digite a atividade #%d (ou <ENTER> para finalizar): ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0)
            break;
        strcpy(novo_plano.atividades[i], buffer);
        novo_plano.total_atividades++;
    }

    // ID e ativação
    snprintf(novo_plano.id, sizeof(novo_plano.id), "%d", total_planos + 1);
    novo_plano.ativo = true;

    lista_planos[total_planos++] = novo_plano;

    // Persistência automática
    salvarPlanos(lista_planos, total_planos);

    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR PLANO                            ===\n");
    printf("=========================================================================\n");
    printf(">>> Plano cadastrado com sucesso! <ENTER>\n");
    getchar();
    limparTela();
}
