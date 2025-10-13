#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"

// Agora usamos .dat pra indicar arquivo binário
#define FUNCIONARIOS_FILE "funcionarios.dat"
#define TMP_FILE_FUNC "funcionarios.tmp"

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
        return 0;

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
