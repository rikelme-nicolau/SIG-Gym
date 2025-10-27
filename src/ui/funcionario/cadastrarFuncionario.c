#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"
#include "../utils/gerarMatricula.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/validarCPF.h"
#include "src/ui/utils/validarEndereco.h"
#include "src/ui/utils/validarEmail.h"
#include "src/ui/utils/validarTelefone.h"
#include "src/ui/utils/validarNascimento.h"

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;

void telaCadastrarFuncionario(void)
{
    if (total_funcionarios >= MAX_FUNCIONARIOS)
    {
        printf("Limite de funcionários atingido!\n");
        getchar();
        return;
    }

    struct funcionario novo_func;
    char buffer[MAX_BUFFER];

    // --- Nome ---
    bool nomeValido = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nDigite o nome: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarNome(buffer))
        {
            strcpy(novo_func.nome, buffer);
            nomeValido = true;
        }
        else
        {
            printf("Nome inválido! Apenas letras e espaços.\nPressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!nomeValido);

    // --- Data de nascimento ---
    bool dataValida = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nNome: %s\nDigite a data de nascimento (DD/MM/AAAA): ", novo_func.nome);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarNascimento(buffer))
        {
            strcpy(novo_func.nascimento, buffer);
            novo_func.idade = calcularIdade(buffer);
            dataValida = true;
        }
        else
        {
            printf("Data inválida! Formato DD/MM/AAAA e não pode ser futura.\nPressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!dataValida);

    // --- CPF ---
    bool cpfValido = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nNome: %s\nDigite o CPF: ", novo_func.nome);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarCPF(buffer))
        {
            strcpy(novo_func.cpf, buffer);
            cpfValido = true;
        }
        else
        {
            printf("CPF inválido! Pressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!cpfValido);

    // --- Telefone ---
    bool telefoneValido = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nDigite o telefone: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarTelefone(buffer))
        {
            strcpy(novo_func.telefone, buffer);
            telefoneValido = true;
        }
        else
        {
            printf("Telefone inválido! Pressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!telefoneValido);

    // --- Endereço ---
    bool enderecoValido = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nDigite o endereço: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarEndereco(buffer))
        {
            strcpy(novo_func.endereco, buffer);
            enderecoValido = true;
        }
        else
        {
            printf("Endereço inválido! Pressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!enderecoValido);

    // --- E-mail ---
    bool emailValido = false;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nDigite o e-mail: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarEmail(buffer))
        {
            strcpy(novo_func.email, buffer);
            emailValido = true;
        }
        else
        {
            printf("E-mail inválido! Pressione ENTER para tentar novamente...");
            getchar();
        }
    } while (!emailValido);

    // --- Cargo ---
    int opcaoCargo = 0;
    do
    {
        limparTela();
        printf("=== CADASTRAR FUNCIONÁRIO ===\nEscolha o cargo:\n1 - Atendente\n2 - Personal\n3 - Gerente\n>>> ");
        fgets(buffer, sizeof(buffer), stdin);
        opcaoCargo = atoi(buffer);
        switch (opcaoCargo)
        {
        case 1:
            strcpy(novo_func.cargo, "Atendente");
            break;
        case 2:
            strcpy(novo_func.cargo, "Personal");
            break;
        case 3:
            strcpy(novo_func.cargo, "Gerente");
            break;
        default:
            printf("Opção inválida! Pressione ENTER para tentar novamente...");
            getchar();
            opcaoCargo = 0;
        }
    } while (opcaoCargo < 1 || opcaoCargo > 3);

    // --- Geração de ID e ativação ---
    strcpy(novo_func.id, gerarMatricula("001"));
    novo_func.ativo = true;
    lista_funcionarios[total_funcionarios++] = novo_func;
    salvarFuncionarios(lista_funcionarios, total_funcionarios);

    // --- Mensagem de sucesso ---
    limparTela();
    printf("=== FUNCIONÁRIO CADASTRADO COM SUCESSO! ===\nPressione ENTER para continuar...");
    getchar();
    limparTela();
}
