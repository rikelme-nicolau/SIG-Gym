#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarFuncionario.h"
#include "arquivoFuncionario.h"

#define FUNCIONARIOS_FILE "funcionarios.txt"
#define TMP_FILE_FUNC "funcionarios.tmp"

// Salva todos os funcionários ativos no arquivo texto
void salvarFuncionarios(struct funcionario lista_funcionarios[], int total_funcionarios)
{
    FILE *fp = fopen(TMP_FILE_FUNC, "wt");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (lista_funcionarios[i].ativo)
        {
            fprintf(fp, "%s;%s;%s;%s;%s;%s;%d\n",
                    lista_funcionarios[i].id,
                    lista_funcionarios[i].nome,
                    lista_funcionarios[i].idade,
                    lista_funcionarios[i].cpf,
                    lista_funcionarios[i].endereco,
                    lista_funcionarios[i].cargo,
                    lista_funcionarios[i].ativo);
        }
    }

    fclose(fp);
    remove(FUNCIONARIOS_FILE);
    rename(TMP_FILE_FUNC, FUNCIONARIOS_FILE);
}

// Carrega todos os funcionários do arquivo texto
int carregarFuncionarios(struct funcionario lista_funcionarios[])
{
    FILE *fp = fopen(FUNCIONARIOS_FILE, "rt");
    if (!fp)
        return 0;

    int total = 0;
    while (!feof(fp) && total < MAX_FUNCIONARIOS)
    {
        fscanf(fp, "%11[^;];%1023[^;];%11[^;];%21[^;];%1023[^;];%1023[^;];%d\n",
               lista_funcionarios[total].id,
               lista_funcionarios[total].nome,
               lista_funcionarios[total].idade,
               lista_funcionarios[total].cpf,
               lista_funcionarios[total].endereco,
               lista_funcionarios[total].cargo,
               (int *)&lista_funcionarios[total].ativo);
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
