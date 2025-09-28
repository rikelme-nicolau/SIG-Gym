#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarAluno.h"

void telaVisualizarAluno(void) {
    if(total_alunos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        VISUALIZAR ALUNO                           ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO CADASTRADO                      ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        VISUALIZAR ALUNO                           ===\n");
    printf("=========================================================================\n");

    for(int i = 0; i < total_alunos; i++) {
        printf("[%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
    }

    printf("\nDigite o ID do aluno que deseja visualizar: ");
    char id_busca[12];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0'; 

    int encontrado = 0;
    for(int i = 0; i < total_alunos; i++) {
        if(strcmp(lista_alunos[i].id, id_busca) == 0) {
            limparTela();
            printf("=========================================================================\n");
            printf("===                        INFORMAÇÕES DO ALUNO                       ===\n");
            printf("=========================================================================\n");
            printf("ID: %s\n", lista_alunos[i].id);
            printf("Nome: %s\n", lista_alunos[i].nome);
            printf("Data de nascimento: %s\n", lista_alunos[i].idade);
            printf("CPF: %s\n", lista_alunos[i].cpf);
            printf("Telefone: %s\n", lista_alunos[i].telefone);
            printf("Endereco: %s\n", lista_alunos[i].endereco);
            printf("Email: %s\n", lista_alunos[i].email);
            printf("=========================================================================\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if(!encontrado) {
        printf("=========================================================================\n");
        printf("===                        VISUALIZAR ALUNO                           ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO ENCONTRADO                      ===\n");
        printf("=========================================================================\n");
    }
    
    getchar();
    limparTela();
}
