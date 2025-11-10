#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"

// Agora usamos .dat pra indicar arquivo binário
#define FUNCIONARIOS_FILE "funcionarios.dat"
#define TMP_FILE_FUNC "funcionarios.tmp"

static int preencherFuncionariosFicticios(struct funcionario lista_funcionarios[])
{
    static const struct funcionario funcionarios_iniciais[] = {
        {
            .id = "FUNC-001",
            .nome = "Ana Martins",
            .nascimento = "15/03/1990",
            .idade = 34,
            .cpf = "111.222.333-44",
            .telefone = "(11) 90001-2233",
            .endereco = "Rua das Acácias, 15 - Centro",
            .email = "ana.martins@example.com",
            .cargo = "Instrutora",
            .ativo = true,
        },
        {
            .id = "FUNC-002",
            .nome = "Bruno Carvalho",
            .nascimento = "08/09/1985",
            .idade = 39,
            .cpf = "555.666.777-88",
            .telefone = "(11) 91122-3344",
            .endereco = "Av. Paulista, 900 - Bela Vista",
            .email = "bruno.carvalho@example.com",
            .cargo = "Personal",
            .ativo = true,
        },
        {
            .id = "FUNC-003",
            .nome = "Clara Nogueira",
            .nascimento = "22/11/1995",
            .idade = 28,
            .cpf = "999.888.777-66",
            .telefone = "(11) 98888-7766",
            .endereco = "Rua das Flores, 300 - Vila Mariana",
            .email = "clara.nogueira@example.com",
            .cargo = "Nutricionista",
            .ativo = true,
        },
    };

    int total = sizeof(funcionarios_iniciais) / sizeof(funcionarios_iniciais[0]);

    for (int i = 0; i < total; i++)
    {
        lista_funcionarios[i] = funcionarios_iniciais[i];
    }

    return total;
}

// Salva todos os funcionários ativos no arquivo binário
void salvarFuncionarios(struct funcionario lista_funcionarios[], int total_funcionarios)
{
    FILE *fp = fopen(TMP_FILE_FUNC, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            // Escreve a struct completa em binário
            fwrite(&lista_funcionarios[i], sizeof(struct funcionario), 1, fp);
        }
    }

    fclose(fp);
    remove(FUNCIONARIOS_FILE);
    rename(TMP_FILE_FUNC, FUNCIONARIOS_FILE);
}

// Carrega todos os funcionários do arquivo binário
int carregarFuncionarios(struct funcionario lista_funcionarios[])
{
    FILE *fp = fopen(FUNCIONARIOS_FILE, "rb");
    if (!fp)
    {
        int total = preencherFuncionariosFicticios(lista_funcionarios);
        salvarFuncionarios(lista_funcionarios, total);
        return total;
    }

    int total = 0;

    while (total < MAX_FUNCIONARIOS && fread(&lista_funcionarios[total], sizeof(struct funcionario), 1, fp) == 1)
    {
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um funcionário específico no arquivo
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

// Marca um funcionário como excluído (exclusão lógica)
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
