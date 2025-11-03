#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h" // Para acessar lista_planos e total_planos

void telaVisualizarAluno(void)
{
    if (total_alunos == 0)
    {
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

    // Listar apenas alunos ativos
    int algum_ativo = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            printf("[%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO ATIVO                           ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    printf("\n>>> Digite o ID do aluno que deseja visualizar: ");
    char id_busca[1024];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\r\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (strcmp(lista_alunos[i].id, id_busca) == 0 && lista_alunos[i].ativo)
        {
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

            // Mostrar nome do plano associado
            char nome_plano[MAX_BUFFER] = "Nenhum";
            if (strcmp(lista_alunos[i].plano_id, "0") != 0)
            {
                for (int j = 0; j < total_planos; j++)
                {
                    if (lista_planos[j].ativo && strcmp(lista_planos[j].id, lista_alunos[i].plano_id) == 0)
                    {
                        strcpy(nome_plano, lista_planos[j].nome);
                        break;
                    }
                }
            }
            printf("Plano associado: %s\n", nome_plano);

            printf("=========================================================================\n");
            printf("=========================================================================\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("=========================================================================\n");
        printf("===                        VISUALIZAR ALUNO                           ===\n");
        printf("=========================================================================\n");
        printf("===                      NENHUM ALUNO ENCONTRADO                      ===\n");
        printf("=========================================================================\n");
    }

    getchar();
    limparTela();
}
