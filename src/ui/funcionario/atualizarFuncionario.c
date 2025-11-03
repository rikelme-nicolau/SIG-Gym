#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/validarCPF.h"
#include "src/ui/utils/validarNascimento.h"
#include "src/ui/utils/validarEndereco.h"
#include "src/ui/utils/validarEmail.h"
#include "src/ui/utils/validarTelefone.h"
#include "src/ui/aluno/atualizarAluno.h"

#define MAX_BUFFER 1024

void telaAtualizarFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        limparTela();
        printf("=== ATUALIZAR FUNCIONÁRIO ===\nNenhum funcionário cadastrado.\n");
        getchar();
        return;
    }

    limparTela();
    printf("=== ATUALIZAR FUNCIONÁRIO ===\nFuncionários disponíveis:\n");
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
            printf("[%s] %s\n", lista_funcionarios[i].id, lista_funcionarios[i].nome);
    }

    printf("Digite o ID do funcionário: ");
    char id_busca[MAX_BUFFER];
    fgets(id_busca, sizeof(id_busca), stdin);
    id_busca[strcspn(id_busca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(lista_funcionarios[i].id, id_busca) == 0 && lista_funcionarios[i].ativo)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        printf("ID não encontrado! Pressione ENTER para continuar...");
        getchar();
        return;
    }

    struct funcionario *func_sel = &lista_funcionarios[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        limparTela();
        printf("=== ATUALIZAR FUNCIONÁRIO ===\n");
        printf("Funcionário selecionado: %s (%s)\n", func_sel->nome, func_sel->id);
        printf("Escolha o campo para atualizar:\n");
        printf("[1] Nome\n[2] Data de nascimento\n[3] CPF\n[4] Telefone\n[5] Endereço\n[6] E-mail\n[7] Cargo\n[0] Voltar\n>>> ");
        scanf(" %c", &opcao);
        limparBufferEntrada();

        switch (opcao)
        {
        case '1': // Nome
            do
            {
                printf("Nome atual: %s\nDigite o novo nome: ", func_sel->nome);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarNome(buffer))
                {
                    strcpy(func_sel->nome, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("Nome inválido! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '2': // Nascimento
            do
            {
                printf("Data atual: %s\nDigite a nova data (DD/MM/AAAA): ", func_sel->nascimento);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarNascimento(buffer))
                {
                    strcpy(func_sel->nascimento, buffer);
                    func_sel->idade = calcularIdade(buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("Data inválida! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '3': // CPF
            do
            {
                printf("CPF atual: %s\nDigite o novo CPF: ", func_sel->cpf);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarCPF(buffer))
                {
                    strcpy(func_sel->cpf, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("CPF inválido! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '4': // Telefone
            do
            {
                printf("Telefone atual: %s\nDigite o novo telefone: ", func_sel->telefone);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarTelefone(buffer))
                {
                    strcpy(func_sel->telefone, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("Telefone inválido! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '5': // Endereço
            do
            {
                printf("Endereço atual: %s\nDigite o novo endereço: ", func_sel->endereco);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarEndereco(buffer))
                {
                    strcpy(func_sel->endereco, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("Endereço inválido! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '6': // E-mail
            do
            {
                printf("E-mail atual: %s\nDigite o novo e-mail: ", func_sel->email);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarEmail(buffer))
                {
                    strcpy(func_sel->email, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    break;
                }
                printf("E-mail inválido! Pressione ENTER para tentar novamente...");
                getchar();
            } while (true);
            break;

        case '7': // Cargo
            do
            {
                printf("Cargo atual: %s\nEscolha o novo cargo:\n1 - Atendente\n2 - Personal\n3 - Gerente\n>>> ", func_sel->cargo);
                fgets(buffer, sizeof(buffer), stdin);
                int escolha = atoi(buffer);
                if (escolha == 1)
                    strcpy(func_sel->cargo, "Atendente");
                else if (escolha == 2)
                    strcpy(func_sel->cargo, "Personal");
                else if (escolha == 3)
                    strcpy(func_sel->cargo, "Gerente");
                else
                {
                    printf("Opção inválida! Pressione ENTER para tentar novamente...");
                    getchar();
                    continue;
                }
                atualizarFuncionarioNoArquivo(*func_sel);
                break;
            } while (true);
            break;

        case '0':
            return;

        default:
            printf("Opção inválida! Pressione ENTER para continuar...");
            getchar();
            break;
        }

        printf("Campo atualizado com sucesso! Pressione ENTER para continuar...");
        getchar();

    } while (true);
}
