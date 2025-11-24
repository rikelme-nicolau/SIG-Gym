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
#include "src/ui/utils/validarEmail.h" // Adicionado para a validacao do e-mail
#include "arquivoAluno.h"
#include "ui/utils/consoleLayout.h"
#include "../utils/geradorDados.h"
#include "../funcionario/cadastrarFuncionario.h"

#define MAX_BUFFER 1024
#define MAX_ALUNOS 1024

/* Tela de cadastro de alunos.
   Conduz o usuario por cada campo, valida as entradas e grava no arquivo
   binario atraves das rotinas de persistencia. */
struct aluno lista_alunos[MAX_ALUNOS];
int total_alunos = 0;

/* Cabecalhos e rodapes centralizam a formatacao para nao repetir layout. */
static void cabecalho_cadastro(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

static void rodape_cancelamento(void)
{
    ui_line('-');
    ui_text_line("Digite o valor e pressione ENTER (0 para cancelar).");
    ui_text_line(">>> Digite abaixo:");
    ui_line('=');
}

/* Le uma linha crua de stdin e garante string terminada em '\0'. */
static bool lerEntrada(char *destino, size_t tamanho)
{
    if (fgets(destino, tamanho, stdin) == NULL)
    {
        destino[0] = '\0';
        return false;
    }
    return true;
}

static bool desejaCancelar(const char *entrada)
{
    return strcmp(entrada, "0") == 0;
}

/* Mensagem padrao caso o usuario digite 0 em qualquer etapa. */
static bool exibirMensagemCancelamento(const char *entrada)
{
    if (!desejaCancelar(entrada))
    {
        return false;
    }

    cabecalho_cadastro("Cadastro cancelado");
    ui_center_text("Operacao interrompida pelo usuario.");
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
    return true;
}

/* Fluxo principal: pergunta campo a campo, valida cada entrada e
   so avanca quando o dado estiver correto ou o usuario cancelar. */
void telaCadastrarAluno(void)
{
    if (total_alunos >= MAX_ALUNOS)
    {
        cabecalho_cadastro("Cadastrar aluno");
        ui_center_text("Limite de alunos atingido.");
        ui_section_title("Pressione <ENTER> para voltar");
        getchar();
        limparTela();
        return;
    }

    struct aluno novo_aluno = {0};
    char buffer[MAX_BUFFER];
    strcpy(novo_aluno.plano_id, "0");

    /* Validacao do nome (somente letras e espacos). */
    bool nomeValido = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        ui_text_line("Informe o nome completo do aluno.");
        ui_text_line("Use apenas letras e espacos.");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarNome(buffer))
        {
            nomeValido = true;
            strcpy(novo_aluno.nome, buffer);
        }
        else
        {
            cabecalho_cadastro("Nome invalido");
            ui_text_line("O nome deve conter apenas letras e espacos.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!nomeValido);

    /* Data de nascimento no formato DD/MM/AAAA e nao futura. */
    bool dataValida = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        char contexto[UI_INNER + 1];
        snprintf(contexto, sizeof(contexto), "Nome: %-.*s", UI_INNER - 6, novo_aluno.nome);
        ui_text_line(contexto);
        ui_empty_line();
        ui_text_line("Digite a data de nascimento (DD/MM/AAAA).");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarNascimento(buffer))
        {
            strcpy(novo_aluno.idade, buffer);
            dataValida = true;
        }
        else
        {
            cabecalho_cadastro("Data invalida");
            ui_text_line("Formato esperado: DD/MM/AAAA e data nao pode ser futura.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!dataValida);
    
    /* CPF precisa ser valido e unico no universo de alunos + funcionarios. */
    bool cpfValido = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        char contexto[UI_INNER + 1];
        snprintf(contexto, sizeof(contexto), "Nome: %-.*s", UI_INNER - 6, novo_aluno.nome);
        ui_text_line(contexto);
        ui_empty_line();
        ui_text_line("Digite o CPF.");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarCPF(buffer))
        {
            if (verificarCPFUnico(buffer, lista_alunos, total_alunos, lista_funcionarios, total_funcionarios))
            {
                strcpy(novo_aluno.cpf, buffer);
                cpfValido = true;
            }
            else
            {
                cabecalho_cadastro("CPF duplicado");
                ui_text_line("CPF ja cadastrado para outro registro.");
                ui_section_title("Pressione <ENTER> para tentar novamente");
                getchar();
            }
        }
        else
        {
            cabecalho_cadastro("CPF invalido");
            ui_text_line("O numero digitado nao corresponde a um CPF valido.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!cpfValido);

    /* Telefone: aceita fixo ou celular com mascara simples. */
    bool telefoneValido = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        ui_text_line("Telefone no formato (DD) 9XXXX-XXXX.");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarTelefone(buffer))
        {
            strcpy(novo_aluno.telefone, buffer);
            telefoneValido = true;
        }
        else
        {
            cabecalho_cadastro("Telefone invalido");
            ui_text_line("Use um numero fixo (10 digitos) ou celular (11 digitos).");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!telefoneValido);

    /* Endereco: texto basico sem caracteres invalidos. */
    bool enderecoValido = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        ui_text_line("Digite o endereco completo (Rua, numero - Bairro).");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarEndereco(buffer))
        {
            strcpy(novo_aluno.endereco, buffer);
            enderecoValido = true;
        }
        else
        {
            cabecalho_cadastro("Endereco invalido");
            ui_text_line("O endereco esta curto ou com caracteres invalidos.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!enderecoValido);

    /* E-mail: valida formato e garante unicidade. */
    bool emailValido = false;
    do
    {
        cabecalho_cadastro("Cadastrar aluno");
        ui_text_line("Digite o e-mail (ex: nome@dominio.com).");
        rodape_cancelamento();
        if (!lerEntrada(buffer, sizeof(buffer)))
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (exibirMensagemCancelamento(buffer))
        {
            return;
        }

        if (validarEmail(buffer))
        {
            if (verificarEmailUnico(buffer, lista_alunos, total_alunos, lista_funcionarios, total_funcionarios))
            {
                strcpy(novo_aluno.email, buffer);
                emailValido = true;
            }
            else
            {
                cabecalho_cadastro("E-mail duplicado");
                ui_text_line("E-mail ja cadastrado para outro registro.");
                ui_section_title("Pressione <ENTER> para tentar novamente");
                getchar();
            }
        }
        else
        {
            cabecalho_cadastro("E-mail invalido");
            ui_text_line("Verifique o formato: texto@provedor.dominio.");
            ui_section_title("Pressione <ENTER> para tentar novamente");
            getchar();
        }
    } while (!emailValido);

    /* Chegando aqui, todos os campos estao OK. Gera ID, marca ativo
       e persiste no arquivo. */
    strcpy(novo_aluno.id, gerarMatricula("002"));
    novo_aluno.ativo = true;
    if (novo_aluno.plano_id[0] == '\0')
    {
        strcpy(novo_aluno.plano_id, "0");
    }
    lista_alunos[total_alunos++] = novo_aluno;
    salvarAlunos(lista_alunos, total_alunos);

    cabecalho_cadastro("Cadastrar aluno");
    ui_center_text("Aluno cadastrado com sucesso.");
    char id_line[UI_INNER + 1];
    snprintf(id_line, sizeof(id_line), "ID gerado: %-.*s", UI_INNER - 11, novo_aluno.id);
    ui_center_text(id_line);
    ui_section_title("Pressione <ENTER> para voltar");
    getchar();
    limparTela();
}
