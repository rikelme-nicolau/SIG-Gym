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
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

#define FC_COL_ID 10
#define FC_COL_NOME 22
#define FC_COL_CARGO 14
#define FC_COL_STATUS 8
#define MAX_BUFFER 1024

static void cabecalho_atualizar(const char *sub)
{
    limparTela();
    ui_header("SIG-GYM", sub);
    ui_empty_line();
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

static void mensagem(const char *sub, const char *texto)
{
    cabecalho_atualizar(sub);
    ui_text_line(texto);
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}

static void tabela_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", FC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", FC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Cargo", FC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", FC_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

static void tabela_row(const struct funcionario *f)
{
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, f->id, FC_COL_ID);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->nome, FC_COL_NOME);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->cargo, FC_COL_CARGO);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, f->ativo ? "Ativo" : "Inativo", FC_COL_STATUS);
    linha[pos] = '\0';
    ui_text_line(linha);
}

void telaAtualizarFuncionario(void)
{
    if (total_funcionarios == 0)
    {
        mensagem("Atualizar funcionario", "Nenhum funcionario cadastrado.");
        return;
    }

    cabecalho_atualizar("Atualizar funcionario");
    ui_text_line("Funcionarios ativos (selecione pelo ID):");
    tabela_header();
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            tabela_row(&lista_funcionarios[i]);
        }
    }

    ui_line('-');
    ui_text_line("Digite o ID do funcionario que deseja atualizar.");
    ui_text_line(">>> Digite abaixo e pressione ENTER.");
    ui_line('=');
    char id_busca[MAX_BUFFER];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }

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
        mensagem("Atualizar funcionario", "ID nao encontrado ou inativo.");
        return;
    }

    struct funcionario *func_sel = &lista_funcionarios[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        cabecalho_atualizar("Atualizar funcionario");
        char linha_sel[UI_INNER + 1];
        snprintf(linha_sel, sizeof(linha_sel), "Funcionario: %s (%s)", func_sel->nome, func_sel->id);
        ui_text_line(linha_sel);
        ui_empty_line();
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Data de nascimento");
        ui_menu_option('3', "CPF");
        ui_menu_option('4', "Telefone");
        ui_menu_option('5', "Endereco");
        ui_menu_option('6', "E-mail");
        ui_menu_option('7', "Cargo");
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha uma opcao");
        printf(">>> ");
        fflush(stdout);
        opcao = lerTecla();

        switch (opcao)
        {
        case '1': // Nome
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "Nome atual: %s", func_sel->nome);
                ui_text_line(linha_sel);
                ui_text_line("Digite o novo nome:");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarNome(buffer))
                {
                    strcpy(func_sel->nome, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "Nome atualizado com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "Nome invalido. Use apenas letras e espacos.");
            }
            break;

        case '2': // Nascimento
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "Data atual: %s", func_sel->nascimento);
                ui_text_line(linha_sel);
                ui_text_line("Digite a nova data (DD/MM/AAAA):");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarNascimento(buffer))
                {
                    strcpy(func_sel->nascimento, buffer);
                    func_sel->idade = calcularIdade(buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "Data atualizada com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "Data invalida. Formato DD/MM/AAAA.");
            }
            break;

        case '3': // CPF
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "CPF atual: %s", func_sel->cpf);
                ui_text_line(linha_sel);
                ui_text_line("Digite o novo CPF:");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarCPF(buffer))
                {
                    strcpy(func_sel->cpf, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "CPF atualizado com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "CPF invalido.");
            }
            break;

        case '4': // Telefone
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "Telefone atual: %s", func_sel->telefone);
                ui_text_line(linha_sel);
                ui_text_line("Digite o novo telefone:");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarTelefone(buffer))
                {
                    strcpy(func_sel->telefone, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "Telefone atualizado com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "Telefone invalido.");
            }
            break;

        case '5': // Endereco
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "Endereco atual: %s", func_sel->endereco);
                ui_text_line(linha_sel);
                ui_text_line("Digite o novo endereco:");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarEndereco(buffer))
                {
                    strcpy(func_sel->endereco, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "Endereco atualizado com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "Endereco invalido.");
            }
            break;

        case '6': // E-mail
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "E-mail atual: %s", func_sel->email);
                ui_text_line(linha_sel);
                ui_text_line("Digite o novo e-mail:");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                if (validarEmail(buffer))
                {
                    strcpy(func_sel->email, buffer);
                    atualizarFuncionarioNoArquivo(*func_sel);
                    mensagem("Atualizar funcionario", "E-mail atualizado com sucesso.");
                    break;
                }
                mensagem("Atualizar funcionario", "E-mail invalido.");
            }
            break;

        case '7': // Cargo
            while (true)
            {
                cabecalho_atualizar("Atualizar funcionario");
                snprintf(linha_sel, sizeof(linha_sel), "Cargo atual: %s", func_sel->cargo);
                ui_text_line(linha_sel);
                ui_text_line("Escolha o novo cargo:");
                ui_menu_option('1', "Atendente");
                ui_menu_option('2', "Personal");
                ui_menu_option('3', "Gerente");
                ui_line('=');
                if (!ler_linha(buffer, sizeof(buffer)))
                    break;
                int escolha = atoi(buffer);
                if (escolha == 1)
                    strcpy(func_sel->cargo, "Atendente");
                else if (escolha == 2)
                    strcpy(func_sel->cargo, "Personal");
                else if (escolha == 3)
                    strcpy(func_sel->cargo, "Gerente");
                else
                {
                    mensagem("Atualizar funcionario", "Opcao invalida. Use 1, 2 ou 3.");
                    continue;
                }
                atualizarFuncionarioNoArquivo(*func_sel);
                mensagem("Atualizar funcionario", "Cargo atualizado com sucesso.");
                break;
            }
            break;

        case '0':
            break;

        default:
            mensagem("Atualizar funcionario", "Opcao invalida. Use o menu.");
            break;
        }

    } while (opcao != '0');

    limparTela();
}
