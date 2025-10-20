#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "../utils/gerarMatricula.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/valdiarCPF.h"
#include "src/ui/utils/validarNascimento.h" // Adicionado para a validação da data
#include "arquivoAluno.h"

#define MAX_BUFFER 1024
#define MAX_ALUNOS 1024

struct aluno lista_alunos[MAX_ALUNOS];
int total_alunos = 0;

void telaCadastrarAluno(void)
{
    if (total_alunos >= MAX_ALUNOS)
    {
        printf("Limite de alunos atingido!\n");
        getchar();
        return;
    }

    struct aluno novo_aluno;
    char buffer[MAX_BUFFER];

    // --- Validação do Nome (já estava correto) ---
    bool nomeValido = false;
    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Por favor, digite o nome:                                         ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarNome(buffer))
        {
            nomeValido = true;
            strcpy(novo_aluno.nome, buffer);
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

    // ======================= MODIFICAÇÃO PARA VALIDAR DATA DE NASCIMENTO =======================
    bool dataValida = false;
    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Nome: %-55s ===\n", novo_aluno.nome);
        printf("=== Por favor, digite a data de nascimento (DD/MM/AAAA):              ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarNascimento(buffer))
        {
            strcpy(novo_aluno.idade, buffer); // O campo 'idade' armazena a data de nascimento
            dataValida = true;
        }
        else
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                   DATA DE NASCIMENTO INVÁLIDA                     ===\n");
            printf("=========================================================================\n");
            printf("=== A data deve estar no formato DD/MM/AAAA, não pode ser futura      ===\n");
            printf("=== e deve ser uma data válida no calendário. Tente novamente.        ===\n");
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para tentar novamente...");
            getchar(); // Pausa para o usuário ler a mensagem
        }
    } while (!dataValida);
    // ======================= FIM DA MODIFICAÇÃO =======================

    // --- Validação do CPF (já estava correto) ---
    bool cpfValido = false;
    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Nome: %-55s ===\n", novo_aluno.nome);
        printf("=== Por favor, digite o CPF:                                          ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarCPF(buffer))
        {
            strcpy(novo_aluno.cpf, buffer);
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

    // --- Telefone ---
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o telefone:                                     ===\n");
    printf("=========================================================================\n");
    printf(">>> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.telefone, buffer);

    // --- Endereço ---
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o endereco:                                     ===\n");
    printf("=========================================================================\n");
    printf(">>> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.endereco, buffer);

    // --- Email ---
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Por favor, digite o email:                                        ===\n");
    printf("=========================================================================\n");
    printf(">>> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.email, buffer);

    // ... (O restante do seu código para Plano, salvar, etc. continua o mesmo) ...

    // Gera ID e define ativo
    strcpy(novo_aluno.id, gerarMatricula("002"));
    novo_aluno.ativo = true;
    lista_alunos[total_alunos++] = novo_aluno;
    salvarAlunos(lista_alunos, total_alunos);

    // Mensagem de sucesso
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                     ALUNO CADASTRADO COM SUCESSO                  ===\n");
    printf("=========================================================================\n");
    printf(">>>press <ENTER>");
    getchar();
    limparTela();
}