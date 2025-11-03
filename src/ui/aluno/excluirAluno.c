#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>           // <-- faltava
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "arquivoAluno.h"

// reutilize as mesmas helpers do módulo de funcionários
static void limparString(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
    for (int i = (int)strlen(str) - 1; i >= 0 && str[i] == ' '; i--)
        str[i] = '\0';
}

static void lerString(char *dest, int tamanho) {
    fgets(dest, tamanho, stdin);
    limparString(dest);
}

void telaExcluirAluno(void)
{
    if (total_alunos == 0) {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        EXCLUIR ALUNO                              ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO CADASTRADO                      ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        EXCLUIR ALUNO                              ===\n");
    printf("=========================================================================\n");

    int algum_ativo = 0;
    for (int i = 0; i < total_alunos; i++) {
        if (lista_alunos[i].ativo) {
            printf("[%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo) {
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO ATIVO                           ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do aluno que deseja excluir: ");
    char id_busca[32];                 // <-- aumente para evitar truncamento
    lerString(id_busca, sizeof(id_busca));

    for (int i = 0; i < total_alunos; i++) {
        // limpe o ID salvo antes de comparar
        char id_aluno[32];
        strncpy(id_aluno, lista_alunos[i].id, sizeof(id_aluno));
        id_aluno[sizeof(id_aluno)-1] = '\0';
        limparString(id_aluno);

        if (strcmp(id_aluno, id_busca) == 0 && lista_alunos[i].ativo) {
            lista_alunos[i].ativo = false;

            // OPÇÃO A: manter sua função (se ela regrava o arquivo corretamente)
            excluirAluno(id_busca);

            // OPÇÃO B (alternativa, mais simétrica ao módulo de funcionários):
            // salvarAlunos(lista_alunos, total_alunos);

            limparTela();
            printf("=========================================================================\n");
            printf("===                        EXCLUIR ALUNO                              ===\n");
            printf("=========================================================================\n");
            printf("===                     ALUNO EXCLUÍDO COM SUCESSO                    ===\n");
            printf("=========================================================================\n");
            getchar();
            limparTela();
            return;
        }
    }

    printf("=========================================================================\n");
    printf("===                        EXCLUIR ALUNO                              ===\n");
    printf("=========================================================================\n");
    printf("===                       ALUNO NÃO ENCONTRADO                        ===\n");
    printf("=========================================================================\n");
    getchar();
    limparTela();
}
