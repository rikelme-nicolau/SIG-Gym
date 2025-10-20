#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "../utils/gerarMatricula.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/validarCPF.h"
#include "src/ui/utils/validarNascimento.h"
#include "src/ui/utils/validarTelefone.h"
#include "src/ui/utils/validarEndereco.h"
#include "src/ui/utils/validarEmail.h" // Adicionado para a validação do e-mail
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

    // --- Validação do Nome (já está correto) ---
    bool nomeValido = false;
    do
    {
        // ... (código existente de validação de nome) ...
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

    // --- Validação da Data de Nascimento (já está correto) ---
    bool dataValida = false;
    do
    {
        // ... (código existente de validação de data) ...
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
            strcpy(novo_aluno.idade, buffer);
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
            getchar();
        }
    } while (!dataValida);
    
    // --- Validação do CPF (já está correto) ---
    bool cpfValido = false;
    do
    {
        // ... (código existente de validação de CPF) ...
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

    // --- Validação do Telefone (já está correto) ---
    bool telefoneValido = false;
    do
    {
        // ... (código existente de validação de telefone) ...
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Por favor, digite o telefone (Ex: (DD) 9XXXX-XXXX):               ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarTelefone(buffer))
        {
            strcpy(novo_aluno.telefone, buffer);
            telefoneValido = true;
        }
        else
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                      NÚMERO DE TELEFONE INVÁLIDO                  ===\n");
            printf("=========================================================================\n");
            printf("=== O número deve ser um telefone fixo (10 dígitos) ou celular      ===\n");
            printf("=== (11 dígitos, começando com 9). Ex: (11) 98765-4321.             ===\n");
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para tentar novamente...");
            getchar();
        }
    } while (!telefoneValido);

    // --- Validação do Endereço (já está correto) ---
    bool enderecoValido = false;
    do
    {
        // ... (código existente de validação de endereço) ...
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Por favor, digite o endereço (Ex: Rua Exemplo, 123 - Centro):     ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarEndereco(buffer))
        {
            strcpy(novo_aluno.endereco, buffer);
            enderecoValido = true;
        }
        else
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                        ENDEREÇO INVÁLIDO                          ===\n");
            printf("=========================================================================\n");
            printf("=== O endereço é muito curto ou contém caracteres inválidos.          ===\n");
            printf("=== Verifique o texto e tente novamente.                              ===\n");
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para tentar novamente...");
            getchar();
        }
    } while (!enderecoValido);

    // ======================= MODIFICAÇÃO PARA VALIDAR E-MAIL =======================
    bool emailValido = false;
    do
    {
        limparTela();
        printf("=========================================================================\n");
        printf("===                        CADASTRAR ALUNO                            ===\n");
        printf("=========================================================================\n");
        printf("=== Por favor, digite o email (Ex: nome@dominio.com):                 ===\n");
        printf("=========================================================================\n");
        printf(">>> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarEmail(buffer))
        {
            strcpy(novo_aluno.email, buffer);
            emailValido = true;
        }
        else
        {
            limparTela();
            printf("=========================================================================\n");
            printf("===                        ENDEREÇO DE E-MAIL INVÁLIDO                ===\n");
            printf("=========================================================================\n");
            printf("=== O formato do e-mail parece incorreto.                           ===\n");
            printf("=== Certifique-se de que ele contém um '@' e um domínio válido.       ===\n");
            printf("=========================================================================\n");
            printf(">>> Pressione <ENTER> para tentar novamente...");
            getchar(); // Pausa para o usuário ler a mensagem
        }
    } while (!emailValido);
    // ======================= FIM DA MODIFICAÇÃO =======================

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