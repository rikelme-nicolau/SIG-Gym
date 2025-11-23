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
#include "ui/utils/consoleLayout.h"

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;

static void cabecalho_func(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

static void rodape_prompt(const char *msg)
{
    ui_line('-');
    ui_text_line(msg);
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
}

static bool ler_linha(char *dest, size_t size)
{
    if (fgets(dest, size, stdin) == NULL)
    {
        dest[0] = '\0';
        return false;
    }
    dest[strcspn(dest, "\n")] = '\0';
    return true;
}

void telaCadastrarFuncionario(void)
{
    if (total_funcionarios >= MAX_FUNCIONARIOS)
    {
        cabecalho_func("Cadastrar funcionario");
        ui_center_text("Limite de funcionarios atingido.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    struct funcionario novo_func = {0};
    char buffer[MAX_BUFFER];

    // --- Nome ---
    bool nomeValido = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        ui_text_line("Digite o nome completo (apenas letras e espacos).");
        rodape_prompt("Nome:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarNome(buffer))
        {
            strcpy(novo_func.nome, buffer);
            nomeValido = true;
        }
        else
        {
            cabecalho_func("Nome invalido");
            ui_text_line("Use apenas letras e espacos.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!nomeValido);

    // --- Data de nascimento ---
    bool dataValida = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        char linha[UI_INNER + 1];
        snprintf(linha, sizeof(linha), "Nome: %-.*s", UI_INNER - 6, novo_func.nome);
        ui_text_line(linha);
        ui_empty_line();
        ui_text_line("Digite a data de nascimento (DD/MM/AAAA).");
        rodape_prompt("Data de nascimento:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarNascimento(buffer))
        {
            strcpy(novo_func.nascimento, buffer);
            novo_func.idade = calcularIdade(buffer);
            dataValida = true;
        }
        else
        {
            cabecalho_func("Data invalida");
            ui_text_line("Formato DD/MM/AAAA e nao pode ser futura.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!dataValida);

    // --- CPF ---
    bool cpfValido = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        char linha[UI_INNER + 1];
        snprintf(linha, sizeof(linha), "Nome: %-.*s", UI_INNER - 6, novo_func.nome);
        ui_text_line(linha);
        ui_empty_line();
        ui_text_line("Digite o CPF.");
        rodape_prompt("CPF:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarCPF(buffer))
        {
            strcpy(novo_func.cpf, buffer);
            cpfValido = true;
        }
        else
        {
            cabecalho_func("CPF invalido");
            ui_text_line("Numero nao corresponde a um CPF valido.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!cpfValido);

    // --- Telefone ---
    bool telefoneValido = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        ui_text_line("Telefone (ex: (11) 98765-4321).");
        rodape_prompt("Telefone:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarTelefone(buffer))
        {
            strcpy(novo_func.telefone, buffer);
            telefoneValido = true;
        }
        else
        {
            cabecalho_func("Telefone invalido");
            ui_text_line("Use fixo (10 digitos) ou celular (11 digitos).");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!telefoneValido);

    // --- Endereço ---
    bool enderecoValido = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        ui_text_line("Endereco completo (Rua, numero - Bairro).");
        rodape_prompt("Endereco:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarEndereco(buffer))
        {
            strcpy(novo_func.endereco, buffer);
            enderecoValido = true;
        }
        else
        {
            cabecalho_func("Endereco invalido");
            ui_text_line("Texto muito curto ou com caracteres invalidos.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!enderecoValido);

    // --- E-mail ---
    bool emailValido = false;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        ui_text_line("Digite o e-mail (ex: nome@dominio.com).");
        rodape_prompt("E-mail:");
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
        if (validarEmail(buffer))
        {
            strcpy(novo_func.email, buffer);
            emailValido = true;
        }
        else
        {
            cabecalho_func("E-mail invalido");
            ui_text_line("Verifique o formato nome@provedor.extensao.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!emailValido);

    // --- Cargo ---
    int opcaoCargo = 0;
    do
    {
        cabecalho_func("Cadastrar funcionario");
        ui_text_line("Escolha o cargo:");
        ui_menu_option('1', "Atendente");
        ui_menu_option('2', "Personal");
        ui_menu_option('3', "Gerente");
        ui_line('=');
        if (!ler_linha(buffer, sizeof(buffer)))
            return;
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
            cabecalho_func("Opcao invalida");
            ui_text_line("Escolha 1, 2 ou 3.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
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
    cabecalho_func("Cadastrar funcionario");
    ui_center_text("Funcionario cadastrado com sucesso!");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
