#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/validarCPF.h"
#include "src/ui/utils/validarNascimento.h"
#include "src/ui/utils/validarTelefone.h"
#include "src/ui/utils/validarEndereco.h"
#include "src/ui/utils/validarEmail.h" // Adicionado para a validação do e-mail
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
        case '1': // Validação do Nome
        {
            // ... (código existente de validação de nome) ...
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
                    printf("=== O nome deve conter apenas letras e espaços.                     ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!nomeValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '2': // Validação da Data de Nascimento
        {
            // ... (código existente de validação de data) ...
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

                if (validarNascimento(buffer))
                {
                    strcpy(aluno_sel->idade, buffer);
                    dataValida = true;
                }
                else
                {
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                   DATA DE NASCIMENTO INVÁLIDA                     ===\n");
                    printf("=========================================================================\n");
                    printf("=== Formato DD/MM/AAAA. A data não pode ser futura.               ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!dataValida);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }

        case '3': // Validação do CPF
        {
            // ... (código existente de validação de CPF) ...
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
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!cpfValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '4': // Validação do Telefone
        {
            // ... (código existente de validação de telefone) ...
            bool telefoneValido = false;
            do
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Telefone atual: %-49s ===\n", aluno_sel->telefone);
                printf("=== Digite o novo telefone (Ex: (DD) 9XXXX-XXXX):                     ===\n");
                printf("=========================================================================\n");
                printf(">>> ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                if (validarTelefone(buffer))
                {
                    strcpy(aluno_sel->telefone, buffer);
                    telefoneValido = true;
                }
                else
                {
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                      NÚMERO DE TELEFONE INVÁLIDO                  ===\n");
                    printf("=========================================================================\n");
                    printf("=== O número deve ser um telefone fixo (10) ou celular (11 dígitos).===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!telefoneValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '5': // Validação do Endereço
        {
            // ... (código existente de validação de endereço) ...
            bool enderecoValido = false;
            do
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== Endereço atual: %-50s ===\n", aluno_sel->endereco);
                printf("=== Digite o novo endereço (Ex: Rua Exemplo, 123 - Centro):           ===\n");
                printf("=========================================================================\n");
                printf(">>> ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                if (validarEndereco(buffer))
                {
                    strcpy(aluno_sel->endereco, buffer);
                    enderecoValido = true;
                }
                else
                {
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                        ENDEREÇO INVÁLIDO                          ===\n");
                    printf("=========================================================================\n");
                    printf("=== O endereço é muito curto ou contém caracteres inválidos.          ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!enderecoValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }

        // ======================= AGREGAÇÃO DA VALIDAÇÃO DE E-MAIL =======================
        case '6':
        {
            bool emailValido = false;
            do
            {
                limparTela();
                printf("=========================================================================\n");
                printf("===                        ATUALIZAR ALUNO                            ===\n");
                printf("=========================================================================\n");
                printf("=== E-mail atual: %-51s ===\n", aluno_sel->email);
                printf("=== Digite o novo e-mail (Ex: nome@dominio.com):                      ===\n");
                printf("=========================================================================\n");
                printf(">>> ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                // Chama a função de validação de e-mail
                if (validarEmail(buffer))
                {
                    strcpy(aluno_sel->email, buffer); // Copia o novo e-mail válido
                    emailValido = true;              // Libera a saída do loop
                }
                else
                {
                    // Exibe mensagem de erro se o e-mail for inválido
                    limparTela();
                    printf("=========================================================================\n");
                    printf("===                        ENDEREÇO DE E-MAIL INVÁLIDO                ===\n");
                    printf("=========================================================================\n");
                    printf("=== O formato do e-mail parece incorreto.                           ===\n");
                    printf("=== Certifique-se de que ele contém um '@' e um domínio válido.       ===\n");
                    printf("=========================================================================\n");
                    printf(">>> Pressione <ENTER> para tentar novamente...");
                    getchar();
                }
            } while (!emailValido);

            atualizarAlunoNoArquivo(*aluno_sel); // <-- salva alteração
            break;
        }
        // ======================= FIM DA AGREGAÇÃO =======================
        
        // ... (Aqui entraria a lógica para o Plano) ...
        
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

        if (opcao > '0' && opcao < '8')
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