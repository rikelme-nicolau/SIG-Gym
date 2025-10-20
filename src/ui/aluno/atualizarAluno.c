#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/utils/validarNome.h" // Adicionado para garantir que a função esteja disponível

#include "arquivoAluno.h" // <-- persistência

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void telaAtualizarAluno(void)
{
    if (total_alunos == 0)
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("===                       NENHUM ALUNO CADASTRADO                     ===\n");
        printf("=========================================================================\n");
        getchar();
        limparTela();
        return;
    }

    limparTela();
    printf("=========================================================================\n");
    printf("===                        ATUALIZAR ALUNO                            ===\n");
    printf("=========================================================================\n");

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            printf("[%s] %s\n", lista_alunos[i].id, lista_alunos[i].nome);
        }
    }

    printf("===                          DIGITE O ID:                             ===\n");
    printf("=========================================================================\n");
    char id_busca[1024];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < total_alunos; i++)
    {
        if (strcmp(lista_alunos[i].id, id_busca) == 0 && lista_alunos[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
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

    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        ATUALIZAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("Aluno selecionado: %s (%s)\n", aluno_sel->nome, aluno_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n");
        printf("[2] Data de nascimento\n");
        printf("[3] CPF\n");
        printf("[4] Telefone\n");
        printf("[5] Endereco\n");
        printf("[6] Email\n");
        printf("[7] Plano\n");
        printf("[0] Voltar\n");
        printf("=========================================================================\n");
        printf("=========================================================================\n");
        scanf(" %c", &opcao);
        limparBufferEntrada();
        switch (opcao)
        {
        // ======================= MODIFICAÇÃO COMEÇA AQUI =======================
        case '1':
        { // Adiciona chaves para criar um escopo local para a variável nomeValido
            bool nomeValido = false;
            do
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Nome atual: %-51s ===\n", aluno_sel->nome);
                printf("=== Digite o novo nome:                                               ===\n");
                printf("=========================================================================\n");
                printf(">>> ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                // Chama a função de validação
                if (validarNome(buffer))
                {
                    strcpy(aluno_sel->nome, buffer); // Copia o novo nome válido
                    nomeValido = true;               // Libera a saída do loop
                }
                else
                {
                    // Exibe mensagem de erro se o nome for inválido
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                        NOME INVÁLIDO                              ===\n");
                    printf("=========================================================================\n");
                    printf("=== O nome deve conter apenas letras e espaços, sem caracteres      ===\n");
                    printf("=== especiais ou números. Verifique também o tamanho.               ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!nomeValido);

            atualizarAlunoNoArquivo(*aluno_sel); // <-- salva alteração
            break;
        }
        // ======================= MODIFICAÇÃO TERMINA AQUI =======================

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

            atualizarAlunoNoArquivo(*aluno_sel);

            break;
        case '3':
            limparTela();
            printf("=========================================================================\n");
            printf("===                        ATUALIZAR ALUNO                            ===\n");
            printf("=========================================================================\n");
            printf("=== Novo CPF:                                                         ===\n");
            printf("=========================================================================\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(aluno_sel->cpf, buffer);
            atualizarAlunoNoArquivo(*aluno_sel);

            break;
        
        // ... (o restante do seu código 'case 4', '5', '6', etc. permanece o mesmo) ...
        
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

        if (opcao > '0' && opcao <= '7') // Mensagem de sucesso apenas para opções válidas
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                        ATUALIZAR ALUNO                            ===\n");
            printf("=========================================================================\n");
            printf("=== Campo atualizado com sucesso! <ENTER>                             ===\n");
            printf("=========================================================================\n");
            getchar();
        }

    } while (opcao != '0');

    limparTela();
}