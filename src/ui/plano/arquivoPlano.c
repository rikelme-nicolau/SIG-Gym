#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarPlano.h"
#include "arquivoPlano.h"

#define PLANOS_FILE "planos.dat"
#define TMP_FILE "planos.tmp"

// Salva todos os planos ativos no arquivo binário
void salvarPlanos(struct plano lista_planos[], int total_planos)
{
    FILE *fp = fopen(TMP_FILE, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            // Escreve a struct inteira de uma vez
            fwrite(&lista_planos[i], sizeof(struct plano), 1, fp);
        }
    }

    fclose(fp);
    remove(PLANOS_FILE);
    rename(TMP_FILE, PLANOS_FILE);
}

// Carrega todos os planos do arquivo binário
int carregarPlanos(struct plano lista_planos[])
{
    FILE *fp = fopen(PLANOS_FILE, "rb");
    if (!fp)
        return 0;

    int total = 0;

    // Lê structs completas até o fim do arquivo
    while (total < MAX_PLANOS && fread(&lista_planos[total], sizeof(struct plano), 1, fp) == 1)
    {
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um plano específico no arquivo
void atualizarPlanoNoArquivo(struct plano plano)
{
    struct plano planos[MAX_PLANOS];
    int total = carregarPlanos(planos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(planos[i].id, plano.id) == 0)
        {
            planos[i] = plano;
            break;
        }
    }

    salvarPlanos(planos, total);
}

// Marca um plano como excluído (exclusão lógica)
void excluirPlano(char *id)
{
    struct plano planos[MAX_PLANOS];
    int total = carregarPlanos(planos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(planos[i].id, id) == 0)
        {
            planos[i].ativo = false;
            break;
        }
    }

    salvarPlanos(planos, total);
}
