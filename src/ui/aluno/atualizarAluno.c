#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "limparTela.h"
#include "cadastrarAluno.h"

void telaAtualizarAluno(void) {
    if(total_alunos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("===                       ID  NAO  ENCONTRADO                         ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        ATUALIZAR ALUNO                            ===\n");
    printf("=========================================================================\n");

    // Listar IDs e nomes apenas dos ativos
    for(int i = 0; i < total_alunos; i++) {
        if(lista_alunos[i].ativo) {
            printf("[%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
        }
    }

    printf("===                          DIGITE O ID:                             ===\n");
    printf("=========================================================================\n");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    // Procurar aluno pelo ID apenas se estiver ativo
    int encontrado = -1;
    for(int i = 0; i < total_alunos; i++) {
        if(strcmp(lista_alunos[i].id, id_busca) == 0 && lista_alunos[i].ativo) {
            encontrado = i;
            break;
        }
    }

    if(encontrado == -1) {
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("===                       ID  NAO  ENCONTRADO                         ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    struct aluno *aluno_sel = &lista_alunos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR ALUNO                            ===\n");
        printf("=========================================================================\n");  
        printf("aluno selecionado: %s (%s)\n", aluno_sel->nome, aluno_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n");
        printf("[2] Data de nascimento\n");
        printf("[3] CPF\n");
        printf("[4] Telefone\n");
        printf("[5] Endereco\n");
        printf("[6] Email\n");
        printf("[0] Voltar\n");
        printf("=========================================================================\n"); 
        printf("=========================================================================\n"); 
        scanf(" %c", &opcao);
        getchar();

        switch(opcao) {
            case '1':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo nome:                                                        ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->nome, buffer);
                break;
            case '2':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Nova data de nascimento:                                          ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->idade, buffer);
                break;
            case '3':
                limparTela();    
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo cpf:                                                         ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->cpf, buffer);
                break;
            case '4':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo telefone:                                                    ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->telefone, buffer);
                break;
            case '5':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo endereço:                                                    ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->endereco, buffer);
                break;
            case '6':
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Novo email:                                                       ===\n");
                printf("=========================================================================\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(aluno_sel->email, buffer);
                break;
            case '0':
                break;
            default:
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("===                        OPCAO  INVALIDA                            ===\n");
                printf("=========================================================================\n");
                break;
        }

        if(opcao != '0') {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Atualizado com sucesso ! <enter>                                  ===\n");
                printf("=========================================================================\n");
            getchar();
        }

    } while(opcao != '0');

    limparTela();
}
