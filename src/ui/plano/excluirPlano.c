#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"
#include "arquivoPlano.h"  // <-- persistência

void telaExcluirPlano(void) {
    if(total_planos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        EXCLUIR PLANO                              ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM PLANO CADASTRADO                      ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        EXCLUIR PLANO                              ===\n");
    printf("=========================================================================\n");

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

    printf("\n>>>digite o ID do plano que deseja excluir: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = 0;
    for(int i = 0; i < total_planos; i++) {
        if(strcmp(lista_planos[i].id, id_busca) == 0 && lista_planos[i].ativo) {
            lista_planos[i].ativo = false; 
            // **Persistência automática**
            excluirPlano(id_busca);

            limparTela();
            printf("=========================================================================\n");
            printf("===                        EXCLUIR PLANO                              ===\n");
            printf("=========================================================================\n");
            printf("===                     PLANO EXCLUIDO COM SUCESSO                    ===\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if(!encontrado) {
        printf("=========================================================================\n");
        printf("===                        EXCLUIR PLANO                              ===\n");
        printf("=========================================================================\n");
        printf("===                     PLANO NAO ENCONTRADO                          ===\n");
        printf("=========================================================================\n");
    }

    printf("\n>>>press <ENTER>");
    getchar();
    limparTela();
}
