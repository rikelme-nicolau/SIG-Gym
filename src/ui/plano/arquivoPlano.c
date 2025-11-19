#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarPlano.h"
#include "arquivoPlano.h"

#define PLANOS_FILE "planos.dat"
#define TMP_FILE "planos.tmp"

struct plano_legacy
{
    char id[12];
    char nome[MAX_BUFFER];
    char horario_inicio[12];
    char horario_fim[12];
    char atividades[MAX_ATIVIDADES][MAX_BUFFER];
    int total_atividades;
    bool ativo;
};

static void converterPlanoLegacy(struct plano *dest, const struct plano_legacy *src)
{
    memset(dest, 0, sizeof(*dest));
    memcpy(dest->id, src->id, sizeof(dest->id));
    memcpy(dest->nome, src->nome, sizeof(dest->nome));
    memcpy(dest->horario_inicio, src->horario_inicio, sizeof(dest->horario_inicio));
    memcpy(dest->horario_fim, src->horario_fim, sizeof(dest->horario_fim));
    for (int i = 0; i < MAX_ATIVIDADES; i++)
    {
        memcpy(dest->atividades[i], src->atividades[i], sizeof(dest->atividades[i]));
    }
    dest->total_atividades = src->total_atividades;
    dest->valor = 0.0;
    dest->ativo = src->ativo;
}

static int preencherPlanosFicticios(struct plano lista_planos[])
{
    static const struct plano planos_iniciais[] = {
        {
            .id = "PLAN-001",
            .nome = "Plano Performance",
            .horario_inicio = "06:00",
            .horario_fim = "10:00",
            .valor = 159.90,
            .atividades = {
                "Musculação",
                "Funcional",
                "Cardio HIIT",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-002",
            .nome = "Plano Bem-Estar",
            .horario_inicio = "10:00",
            .horario_fim = "14:00",
            .valor = 129.90,
            .atividades = {
                "Pilates",
                "Alongamento",
                "Yoga",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-003",
            .nome = "Plano Premium",
            .horario_inicio = "16:00",
            .horario_fim = "22:00",
            .valor = 219.90,
            .atividades = {
                "Cross Training",
                "Spinning",
                "Natação",
            },
            .total_atividades = 3,
            .ativo = true,
        },
    };

    int total = sizeof(planos_iniciais) / sizeof(planos_iniciais[0]);

    for (int i = 0; i < total; i++)
    {
        lista_planos[i] = planos_iniciais[i];
    }

    return total;
}

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
    {
        int total = preencherPlanosFicticios(lista_planos);
        salvarPlanos(lista_planos, total);
        return total;
    }

    long file_size = 0;
    if (fseek(fp, 0, SEEK_END) == 0)
    {
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    int total = 0;

    if (file_size >= 0 && (file_size % (long)sizeof(struct plano)) == 0)
    {
        while (total < MAX_PLANOS && fread(&lista_planos[total], sizeof(struct plano), 1, fp) == 1)
        {
            total++;
        }
        fclose(fp);
        return total;
    }

    if (file_size >= 0 && (file_size % (long)sizeof(struct plano_legacy)) == 0)
    {
        while (total < MAX_PLANOS)
        {
            struct plano_legacy registro_antigo;
            if (fread(&registro_antigo, sizeof(registro_antigo), 1, fp) != 1)
            {
                break;
            }
            converterPlanoLegacy(&lista_planos[total], &registro_antigo);
            total++;
        }
        fclose(fp);
        salvarPlanos(lista_planos, total);
        return total;
    }

    fclose(fp);
    int total_padrao = preencherPlanosFicticios(lista_planos);
    salvarPlanos(lista_planos, total_padrao);
    return total_padrao;
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
