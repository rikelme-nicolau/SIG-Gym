#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"  // arquivo onde está struct plano e lista_planos

void telaVisualizarPlano(void) {
    if(total_planos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        VISUALIZAR PLANO                           ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM PLANO CADASTRADO                      ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        VISUALIZAR PLANO                           ===\n");
    printf("=========================================================================\n");

    // Listar apenas planos ativos
    int algum_ativo = 0;
    for(int i = 0; i < total_planos; i++) {
        if(lista_planos[i].ativo) {
            printf("[%s] %s\n", lista_planos[i].id, lista_planos[i].nome);
            algum_ativo = 1;
        }
    }

    if(!algum_ativo) {
        printf("=========================================================================\n");
        printf("===                      NENHUM PLANO ATIVO                           ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do plano que deseja visualizar: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0'; 

    int encontrado = 0;
    for(int i = 0; i < total_planos; i++) {
        if(strcmp(lista_planos[i].id, id_busca) == 0 && lista_planos[i].ativo) {
            limparTela();
            printf("=========================================================================\n");
            printf("===                       DETALHES DO PLANO                           ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_planos[i].id);
            printf("Nome: %s\n", lista_planos[i].nome);
            printf("Horário de funcionamento: %s às %s\n", 
                   lista_planos[i].horario_inicio, 
                   lista_planos[i].horario_fim);

            printf("Atividades incluídas:\n");
            for(int j = 0; j < lista_planos[i].total_atividades; j++) {
                printf(" - %s\n", lista_planos[i].atividades[j]);
            }
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if(!encontrado) {
        printf("=========================================================================\n");
        printf("===                        VISUALIZAR PLANO                           ===\n");
        printf("=========================================================================\n");
        printf("===                      PLANO NAO ENCONTRADO                         ===\n");
        printf("=========================================================================\n");
    }

    printf("\n>>> Pressione <ENTER>");
    getchar();
    limparTela();
}
