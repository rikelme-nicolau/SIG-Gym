#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "../utils/gerarMatricula.h"
#include "src/ui/utils/validarNome.h" // <<< SUA FUNÇÃO ESTÁ SENDO INCLUÍDA AQUI

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

    // ======================= MODIFICAÇÃO COMEÇA AQUI =======================

    bool nomeValido = false;
    do
    {
        // Nome
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Por favor, digite o nome:                                         ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Chama a sua função de validação que veio do #include
        if (validarNome(buffer))
        {
            nomeValido = true; // Se o nome for bom, a flag vira true e o loop acaba
            strcpy(novo_aluno.nome, buffer); // Copia o nome válido para a struct
        }
        else
        {
            // Se o nome for inválido, mostra uma mensagem de erro e o loop repetirá
            limparTela();
            printf("=========================================================================\n");
            printf("===                        NOME INVÁLIDO                              ===\n");
            printf("=========================================================================\n");
            printf("=== O nome deve conter apenas letras e espaços, sem caracteres      ===\n");
            printf("=== especiais ou números. Verifique também o tamanho.               ===\n");
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para tentar novamente...");
            getchar(); // Pausa para o usuário ler a mensagem
        }

    } while (!nomeValido);

    // ======================= MODIFICAÇÃO TERMINA AQUI =======================

    // Idade
    limparTela();
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("=== Nome: %-55s ===\n", novo_aluno.nome); // Mostra o nome já validado
    printf("=== Por favor, digite a data de nascimento:                           ===\n");
    printf("=========================================================================\n");
    printf(">>> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(novo_aluno.idade, buffer);

    // CPF
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
    strcpy(novo_aluno.cpf, buffer);

    // (O restante do seu código continua aqui, sem alterações...)
    // ... Telefone, Endereço, Email, Plano ...
    
    // Telefone
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

    // Endereço
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

    // Email
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

    // Plano
    // ... (seu código de plano)

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