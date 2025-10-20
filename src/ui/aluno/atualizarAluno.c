#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/valdiarCPF.h"
#include "src/ui/utils/validarNascimento.h" // Adicionado para a validação da data
#include "arquivoAluno.h" // <-- persistência

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void telaAtualizarAluno(void)
{
    // ... (O código de busca do aluno permanece o mesmo) ...
    if (total_alunos == 0)
    {
        // ... (código de erro) ...
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
        // ... (código de erro) ...
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
        case '1':
            // ... (Seu código de validação de NOME, já correto) ...
            {
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
                    if (validarNome(buffer))
                    {
                        strcpy(aluno_sel->nome, buffer);
                        nomeValido = true;
                    }
                    else
                    {
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
                atualizarAlunoNoArquivo(*aluno_sel);
                break;
            }
        
        // ======================= AGREGAÇÃO DA VALIDAÇÃO DA DATA DE NASCIMENTO =======================
        case '2':
        {
            bool dataValida = false;
            do
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Data atual: %-51s ===\n", aluno_sel->idade);
                printf("=== Digite a nova data de nascimento (DD/MM/AAAA):                    ===\n");
                printf("=========================================================================\n");
                printf(">>> ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                // Chama a função de validação de data de nascimento
                if (validarNascimento(buffer))
                {
                    strcpy(aluno_sel->idade, buffer); // O campo 'idade' armazena a data
                    dataValida = true;               // Libera a saída do loop
                }
                else
                {
                    // Exibe mensagem de erro se a data for inválida
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                   DATA DE NASCIMENTO INVÁLIDA                     ===\n");
                    printf("=========================================================================\n");
                    printf("=== A data deve estar no formato DD/MM/AAAA, não pode ser futura      ===\n");
                    printf("=== e deve ser uma data válida no calendário. Tente novamente.        ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!dataValida);

            atualizarAlunoNoArquivo(*aluno_sel); // <-- salva alteração
            break;
        }
        // ======================= FIM DA AGREGAÇÃO =======================

        case '3':
            // ... (Seu código de validação de CPF, já correto) ...
            {
                bool cpfValido = false;
                do
                {
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                        ATUALIZAR ALUNO                            ===\n");
                    printf("=========================================================================\n");
                    printf("=== CPF atual: %-54s ===\n", aluno_sel->cpf);
                    printf("=== Digite o novo CPF:                                                ===\n");
                    printf("=========================================================================\n");
                    printf(">>> ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if (validarCPF(buffer))
                    {
                        strcpy(aluno_sel->cpf, buffer);
                        cpfValido = true;
                    }
                    else
                    {
                        limparTela();
                        printf("=========================================================================\n");
                        printf("===                        CPF INVÁLIDO                               ===\n");
                        printf("=========================================================================\n");
                        printf("=== O número digitado não corresponde a um CPF válido.              ===\n");
                        printf("=== Verifique os dígitos e tente novamente.                         ===\n");
                        printf("=========================================================================\n");
                        printf(">>> Pressione <ENTER> para tentar novamente...");
                        getchar();
                    }
                } while (!cpfValido);
                atualizarAlunoNoArquivo(*aluno_sel);
                break;
            }
        
        // ... (o restante do seu código 'case 4', '5', '6', etc.) ...
        
        case '0':
            break;
        default:
            // ... (código para opção inválida) ...
            break;
        }

        if (opcao > '0' && opcao <= '7')
        {
            // ... (mensagem de sucesso) ...
        }

    } while (opcao != '0');

    limparTela();
}