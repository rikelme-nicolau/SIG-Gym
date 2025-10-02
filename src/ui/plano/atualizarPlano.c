#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarPlano.h"
#include "arquivoPlano.h"  // <-- persistência

void telaAtualizarPlano(void) {
    if(total_planos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR PLANO                            ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM PLANO CADASTRADO                       ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        ATUALIZAR PLANO                            ===\n");
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

    printf("\n>>> Digite o ID do plano que deseja atualizar: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = -1;
    for(int i = 0; i < total_planos; i++) {
        if(strcmp(lista_planos[i].id, id_busca) == 0 && lista_planos[i].ativo) {
            encontrado = i;
            break;
        }
    }

    if(encontrado == -1) {
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR PLANO                            ===\n");
        printf("=========================================================================\n");
        printf("===                     PLANO NAO ENCONTRADO                          ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    struct plano *plano_sel = &lista_planos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR PLANO                            ===\n");
        printf("=========================================================================\n");
        printf("Plano selecionado: %s (%s)\n", plano_sel->nome, plano_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n");
        printf("[2] Horário de funcionamento\n");
        printf("[3] Atividades\n");
        printf("[0] Voltar\n");
        printf("=========================================================================\n");
        scanf(" %c", &opcao);
        getchar();

        switch(opcao) {
            case '1':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR PLANO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo nome:                                                        ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(plano_sel->nome, buffer);
                atualizarPlanoNoArquivo(*plano_sel); // <-- salva alteração
                break;

            case '2':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR PLANO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo horário de início:                                           ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(plano_sel->horario_inicio, buffer);

                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR PLANO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo horário de fim:                                              ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(plano_sel->horario_fim, buffer);

                atualizarPlanoNoArquivo(*plano_sel); // <-- salva alteração

                break;

            case '3':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR PLANO                            ===\n");
                printf("=========================================================================\n");
                plano_sel->total_atividades = 0;
                for(int i = 0; i < MAX_ATIVIDADES; i++) {
                    printf(">>> Digite a atividade #%d (ou ENTER para finalizar): ", i+1);
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if(strlen(buffer) == 0) break;
                    strcpy(plano_sel->atividades[i], buffer);
                    plano_sel->total_atividades++;
                }
                atualizarPlanoNoArquivo(*plano_sel); // <-- salva alteração
                break;

            case '0':
                break;

            default:
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR PLANO                            ===\n");
                printf("=========================================================================\n");
                printf("===                        OPCAO INVALIDA                             ===\n");
                printf("=========================================================================\n");
                break;
        }

        if(opcao != '0') {
            limparTela();
            printf("=========================================================================\n");
            printf("===                        ATUALIZAR PLANO                            ===\n");
            printf("=========================================================================\n");
            printf("=== Atualizado com sucesso! <ENTER>                                   ===\n");
            printf("=========================================================================\n");
            getchar();
        }

    } while(opcao != '0');

    limparTela();
}
