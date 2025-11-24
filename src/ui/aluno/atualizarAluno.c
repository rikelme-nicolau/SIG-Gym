#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarAluno.h"
#include "src/ui/plano/cadastrarPlano.h"
#include "src/ui/funcionario/cadastrarFuncionario.h"
#include "src/ui/utils/validarNome.h"
#include "src/ui/utils/validarCPF.h"
#include "src/ui/utils/validarNascimento.h"
#include "src/ui/utils/validarTelefone.h"
#include "src/ui/utils/validarEndereco.h"
#include "src/ui/utils/validarEmail.h" // Adicionado para a validacao do e-mail
#include "src/ui/utils/geradorDados.h"
#include "arquivoAluno.h" // <-- persistencia
#include "ui/utils/lerTecla.h"
#include "ui/utils/consoleLayout.h"

/* Permite editar um aluno existente campo a campo, reaproveitando
   as validacoes usadas no cadastro e refletindo as mudancas no arquivo. */

/* Helpers de layout para manter as telas consistentes. */
static void cabecalho_atualizar(const char *subtitulo)
{
    limparTela();
    ui_header("SIG-GYM", subtitulo);
    ui_empty_line();
}

/* Leitura simples de linha; evita usar scanf para nao deixar lixo no buffer. */
static bool ler_linha(char *dest, size_t tamanho)
{
    if (fgets(dest, tamanho, stdin) == NULL)
    {
        dest[0] = '\0';
        return false;
    }
    return true;
}

/* Tabelas com cabecalho/linha reaproveitadas em listar e confirmar selecao. */
static void tabela_alunos_header(void)
{
    ui_line('-');
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, "ID", 12);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Nome", 26);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Plano", 17);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, "Status", 8);
    linha[pos] = '\0';
    ui_text_line(linha);
    ui_line('-');
}

/* Monta a linha formatada para exibir um aluno na tabela. */
static void tabela_alunos_row(const char *id, const char *nome, const char *plano, const char *status)
{
    char status_clip[16];
    ui_clip_utf8(status, 8, status_clip, sizeof(status_clip));
    char linha[UI_INNER + 1];
    int pos = 0;
    ui_append_col(linha, sizeof(linha), &pos, id, 12);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, nome, 26);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, plano, 17);
    ui_append_sep(linha, sizeof(linha), &pos);
    ui_append_col(linha, sizeof(linha), &pos, status_clip, 8);
    linha[pos] = '\0';
    ui_text_line(linha);
}

/* Mensagem generica com pausa; evita duplicar blocos de UI. */
static void mensagem_comum(const char *subtitulo, const char *linha1, const char *linha2)
{
    cabecalho_atualizar(subtitulo);
    if (linha1 != NULL)
    {
        ui_text_line(linha1);
    }
    if (linha2 != NULL)
    {
        ui_text_line(linha2);
    }
    ui_section_title("Pressione <ENTER> para continuar");
    getchar();
    limparTela();
}

/* Busca o nome do plano associado para exibir junto do aluno. */
static void preencher_nome_plano(const struct aluno *aluno, char *dest, size_t size)
{
    strncpy(dest, "Sem plano", size - 1);
    dest[size - 1] = '\0';

    if (strcmp(aluno->plano_id, "0") != 0)
    {
        for (int j = 0; j < total_planos; j++)
        {
            if (lista_planos[j].ativo && strcmp(lista_planos[j].id, aluno->plano_id) == 0)
            {
                strncpy(dest, lista_planos[j].nome, size - 1);
                dest[size - 1] = '\0';
                break;
            }
        }
    }
}

/* Menu de atualizacao: permite escolher qual campo editar e valida cada novo valor. */
void telaAtualizarAluno(void)
{
    if (total_alunos == 0)
    {
        mensagem_comum("Atualizar aluno", "Nenhum aluno cadastrado.", NULL);
        return;
    }

    cabecalho_atualizar("Atualizar aluno");
    ui_text_line("Selecione o aluno ativo pelo ID.");
    tabela_alunos_header();

    int algum_ativo = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            char nome_plano[MAX_BUFFER];
            preencher_nome_plano(&lista_alunos[i], nome_plano, sizeof(nome_plano));
            tabela_alunos_row(lista_alunos[i].id, lista_alunos[i].nome, nome_plano, "Ativo");
            algum_ativo = 1;
        }
    }

    if (!algum_ativo)
    {
        ui_section_title("Nenhum aluno ativo");
        getchar();
        limparTela();
        return;
    }

    ui_line('-');
    ui_text_line("Digite o ID do aluno que deseja atualizar.");
    ui_line('=');
    printf(">>> ");
    char id_busca[1024];
    if (!ler_linha(id_busca, sizeof(id_busca)))
    {
        limparTela();
        return;
    }
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
        mensagem_comum("Atualizar aluno", "ID nao encontrado ou inativo.", NULL);
        return;
    }

    struct aluno *aluno_sel = &lista_alunos[encontrado];
    char buffer[MAX_BUFFER];
    char opcao;

    do
    {
        cabecalho_atualizar("Atualizar aluno");
        char linha[UI_INNER + 1];
        snprintf(linha, sizeof(linha), "Aluno: %s (%s)", aluno_sel->nome, aluno_sel->id);
        ui_text_line(linha);
        ui_empty_line();
        ui_menu_option('1', "Nome");
        ui_menu_option('2', "Data de nascimento");
        ui_menu_option('3', "CPF");
        ui_menu_option('4', "Telefone");
        ui_menu_option('5', "Endereco");
        ui_menu_option('6', "E-mail");
        ui_menu_option('7', "Plano (em breve)");
        ui_menu_option('0', "Voltar");
        ui_section_title("Escolha um campo");
        printf(">>> ");
        fflush(stdout);
        opcao = lerTecla();
        switch (opcao)
        {
        case '1': // Validacao do Nome
        {
            bool nomeValido = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "Nome atual: %s", aluno_sel->nome);
                ui_text_line(linha);
                ui_text_line("Digite o novo nome (somente letras e espacos).");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarNome(buffer))
                {
                    strcpy(aluno_sel->nome, buffer);
                    nomeValido = true;
                }
                else
                {
                    mensagem_comum("Nome invalido", "Use apenas letras e espacos.", NULL);
                }
            } while (!nomeValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '2': // Validacao da Data de Nascimento
        {
            bool dataValida = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "Data atual: %s", aluno_sel->idade);
                ui_text_line(linha);
                ui_text_line("Digite a nova data (DD/MM/AAAA).");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';

                if (validarNascimento(buffer))
                {
                    strcpy(aluno_sel->idade, buffer);
                    dataValida = true;
                }
                else
                {
                    mensagem_comum("Data invalida", "Formato DD/MM/AAAA e data nao pode ser futura.", NULL);
                }
            } while (!dataValida);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }

        case '3': // Validacao do CPF
        {
            bool cpfValido = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "CPF atual: %s", aluno_sel->cpf);
                ui_text_line(linha);
                ui_text_line("Digite o novo CPF.");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (validarCPF(buffer))
                {
                    bool cpfDisponivel = (strcmp(buffer, aluno_sel->cpf) == 0) ||
                                         verificarCPFUnico(buffer, lista_alunos, total_alunos, lista_funcionarios, total_funcionarios);
                    if (cpfDisponivel)
                    {
                        strcpy(aluno_sel->cpf, buffer);
                        cpfValido = true;
                    }
                    else
                    {
                        mensagem_comum("CPF duplicado", "CPF ja cadastrado para outro registro.", NULL);
                    }
                }
                else
                {
                    mensagem_comum("CPF invalido", "O numero nao corresponde a um CPF valido.", NULL);
                }
            } while (!cpfValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '4': // Validacao do Telefone
        {
            bool telefoneValido = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "Telefone atual: %s", aluno_sel->telefone);
                ui_text_line(linha);
                ui_text_line("Digite o novo telefone (DD) 9XXXX-XXXX.");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';

                if (validarTelefone(buffer))
                {
                    strcpy(aluno_sel->telefone, buffer);
                    telefoneValido = true;
                }
                else
                {
                    mensagem_comum("Telefone invalido", "Use um fixo (10 digitos) ou celular (11 digitos).", NULL);
                }
            } while (!telefoneValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }
        
        case '5': // Validacao do Endereco
        {
            bool enderecoValido = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "Endereco atual: %s", aluno_sel->endereco);
                ui_text_line(linha);
                ui_text_line("Digite o novo endereco.");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';

                if (validarEndereco(buffer))
                {
                    strcpy(aluno_sel->endereco, buffer);
                    enderecoValido = true;
                }
                else
                {
                    mensagem_comum("Endereco invalido", "O texto esta curto ou com caracteres invalidos.", NULL);
                }
            } while (!enderecoValido);
            atualizarAlunoNoArquivo(*aluno_sel);
            break;
        }

        case '6':
        {
            bool emailValido = false;
            do
            {
                cabecalho_atualizar("Atualizar aluno");
                snprintf(linha, sizeof(linha), "E-mail atual: %s", aluno_sel->email);
                ui_text_line(linha);
                ui_text_line("Digite o novo e-mail (nome@dominio.com).");
                ui_line('=');
                printf(">>> ");
                if (!ler_linha(buffer, sizeof(buffer)))
                {
                    mensagem_comum("Atualizar aluno", "Entrada invalida. Retornando ao menu.", NULL);
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';

                // Chama a funcao de validacao de e-mail
                if (validarEmail(buffer))
                {
                    bool emailDisponivel = (strcmp(buffer, aluno_sel->email) == 0) ||
                                           verificarEmailUnico(buffer, lista_alunos, total_alunos, lista_funcionarios, total_funcionarios);
                    if (emailDisponivel)
                    {
                        strcpy(aluno_sel->email, buffer); // Copia o novo e-mail valido
                        emailValido = true;               // Libera a saida do loop
                    }
                    else
                    {
                        mensagem_comum("E-mail duplicado", "E-mail ja cadastrado para outro registro.", NULL);
                    }
                }
                else
                {
                    mensagem_comum("E-mail invalido", "Confirme o formato nome@provedor.extensao.", NULL);
                }
            } while (!emailValido);

            atualizarAlunoNoArquivo(*aluno_sel); // <-- salva alteracao
            break;
        }
        
        case '7':
            mensagem_comum("Atualizar aluno", "Edicao de plano ainda nao disponivel neste menu.", NULL);
            break;
        
        case '0':
            break;
        default:
            mensagem_comum("Atualizar aluno", "Opcao invalida. Use apenas o menu.", NULL);
            break;
        }

        /* Somente exibe confirmacao quando algo foi alterado. */
        if (opcao >= '1' && opcao <= '6')
        {
            mensagem_comum("Atualizar aluno", "Campo atualizado com sucesso!", NULL);
        }

    } while (opcao != '0');

    limparTela();
}
