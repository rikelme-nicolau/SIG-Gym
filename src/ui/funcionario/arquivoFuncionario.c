#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"
#include "../utils/geradorNomes.h"
#include "../utils/geradorDados.h"
#include "../utils/validarNascimento.h"
#include "../aluno/cadastrarAluno.h"
#include "../utils/logGeracao.h"

#define FUNCIONARIOS_FILE "funcionarios.dat"
#define TMP_FILE_FUNC "funcionarios.tmp"
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
#define TOTAL_FUNCIONARIOS_FICTICIOS 20

static void preencherCargos(char cargos[][20])
{
    int idx = 0;

    for (int i = 0; i < 8 && idx < TOTAL_FUNCIONARIOS_FICTICIOS; i++)
    {
        strncpy(cargos[idx++], "Instrutor", 19);
    }
    for (int i = 0; i < 5 && idx < TOTAL_FUNCIONARIOS_FICTICIOS; i++)
    {
        strncpy(cargos[idx++], "Personal Trainer", 19);
    }
    for (int i = 0; i < 3 && idx < TOTAL_FUNCIONARIOS_FICTICIOS; i++)
    {
        strncpy(cargos[idx++], "Nutricionista", 19);
    }
    for (int i = 0; i < 2 && idx < TOTAL_FUNCIONARIOS_FICTICIOS; i++)
    {
        strncpy(cargos[idx++], "Recepcionista", 19);
    }
    if (idx < TOTAL_FUNCIONARIOS_FICTICIOS)
    {
        strncpy(cargos[idx++], "Gerente", 19);
    }
    if (idx < TOTAL_FUNCIONARIOS_FICTICIOS)
    {
        strncpy(cargos[idx++], "Limpeza", 19);
    }

    for (int i = 0; i < idx; i++)
    {
        cargos[i][19] = '\0';
    }

    for (int i = idx - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char tmp[20];
        memcpy(tmp, cargos[i], sizeof(tmp));
        memcpy(cargos[i], cargos[j], sizeof(cargos[i]));
        memcpy(cargos[j], tmp, sizeof(tmp));
    }
}

static void gerarNascimentoPorCargo(const char *cargo, char *destino)
{
    if (strcmp(cargo, "Gerente") == 0)
    {
        gerarDataNascimentoAleatoria(destino, 35, 60);
        return;
    }
    if (strcmp(cargo, "Personal Trainer") == 0 || strcmp(cargo, "Nutricionista") == 0)
    {
        gerarDataNascimentoAleatoria(destino, 25, 45);
        return;
    }
    if (strcmp(cargo, "Instrutor") == 0)
    {
        gerarDataNascimentoAleatoria(destino, 20, 40);
        return;
    }
    if (strcmp(cargo, "Recepcionista") == 0 || strcmp(cargo, "Limpeza") == 0)
    {
        gerarDataNascimentoAleatoria(destino, 20, 50);
        return;
    }

    gerarDataNascimentoAleatoria(destino, 20, 60);
}

static int preencherFuncionariosFicticios(struct funcionario lista_funcionarios[])
{
    char cargos[TOTAL_FUNCIONARIOS_FICTICIOS][20] = {{0}};
    preencherCargos(cargos);

    int total = 0;
    for (int i = 0; i < TOTAL_FUNCIONARIOS_FICTICIOS && total < MAX_FUNCIONARIOS; i++)
    {
        struct funcionario novo;
        memset(&novo, 0, sizeof(novo));

        snprintf(novo.id, sizeof(novo.id), "FUNC-%03d", i + 1);
        gerarNomeCompletoAleatorio(novo.nome, sizeof(novo.nome));

        int tentativas_cpf = 0;
        do
        {
            gerarCPFAleatorio(novo.cpf);
            tentativas_cpf++;
            if (tentativas_cpf > 100)
            {
                printf("ERRO: Nao conseguiu gerar CPF unico para funcionario.\n");
                break;
            }
        } while (!verificarCPFUnico(novo.cpf, lista_alunos, total_alunos, lista_funcionarios, total));

        gerarTelefoneAleatorio(novo.telefone);
        gerarEnderecoAleatorio(novo.endereco, sizeof(novo.endereco));
        int tentativas_email = 0;
        do
        {
            gerarEmailAleatorio(novo.nome, novo.email);
            tentativas_email++;
            if (tentativas_email > 100)
            {
                printf("ERRO: Nao conseguiu gerar email unico para funcionario.\n");
                break;
            }
        } while (!verificarEmailUnico(novo.email, lista_alunos, total_alunos, lista_funcionarios, total));

        strncpy(novo.cargo, cargos[i], sizeof(novo.cargo) - 1);
        novo.cargo[sizeof(novo.cargo) - 1] = '\0';

        gerarNascimentoPorCargo(novo.cargo, novo.nascimento);
        novo.idade = calcularIdade(novo.nascimento);

        novo.ativo = true;

        lista_funcionarios[total++] = novo;
    }

    int qtd_inativos = 2 + (rand() % 2); // 2 a 3
    int marcados = 0;
    while (marcados < qtd_inativos && marcados < total)
    {
        int idx = rand() % total;
        if (lista_funcionarios[idx].ativo)
        {
            lista_funcionarios[idx].ativo = false;
            marcados++;
        }
    }

    logEtapaGeracao("FUNCIONARIOS", total);

    return total;
}

static int gerarFuncionariosPadrao(struct funcionario lista_funcionarios[])
{
    int total = preencherFuncionariosFicticios(lista_funcionarios);
    salvarFuncionarios(lista_funcionarios, total);
    return total;
}

// Salva todos os funcionarios ativos no arquivo binario
void salvarFuncionarios(struct funcionario lista_funcionarios[], int total_funcionarios)
{
    FILE *fp = fopen(TMP_FILE_FUNC, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporario");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            fwrite(&lista_funcionarios[i], sizeof(struct funcionario), 1, fp);
        }
    }

    fclose(fp);
    remove(FUNCIONARIOS_FILE);
    rename(TMP_FILE_FUNC, FUNCIONARIOS_FILE);
}

// Carrega todos os funcionarios do arquivo binario
int carregarFuncionarios(struct funcionario lista_funcionarios[])
{
    FILE *fp = fopen(FUNCIONARIOS_FILE, "rb");
    if (!fp)
    {
        return gerarFuncionariosPadrao(lista_funcionarios);
    }

    long file_size = 0;
    if (fseek(fp, 0, SEEK_END) == 0)
    {
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    if (file_size == 0)
    {
        fclose(fp);
        return gerarFuncionariosPadrao(lista_funcionarios);
    }

    int total = 0;

    while (total < MAX_FUNCIONARIOS && fread(&lista_funcionarios[total], sizeof(struct funcionario), 1, fp) == 1)
    {
        total++;
    }

    fclose(fp);

    if (total == 0)
    {
        return gerarFuncionariosPadrao(lista_funcionarios);
    }

    return total;
}

// Atualiza um funcionario especifico no arquivo
void atualizarFuncionarioNoArquivo(struct funcionario func)
{
    struct funcionario funcionarios[MAX_FUNCIONARIOS];
    int total = carregarFuncionarios(funcionarios);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(funcionarios[i].id, func.id) == 0)
        {
            funcionarios[i] = func;
            break;
        }
    }

    salvarFuncionarios(funcionarios, total);
}

// Marca um funcionario como excluido (exclusao logica)
void excluirFuncionario(char *id)
{
    struct funcionario funcionarios[MAX_FUNCIONARIOS];
    int total = carregarFuncionarios(funcionarios);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(funcionarios[i].id, id) == 0)
        {
            funcionarios[i].ativo = false;
            break;
        }
    }

    salvarFuncionarios(funcionarios, total);
}
