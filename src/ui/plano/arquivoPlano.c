#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarPlano.h"
#include "arquivoPlano.h"
#include "../utils/logGeracao.h"

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
                "Musculacao",
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
                "Natacao",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-004",
            .nome = "Plano Matinal",
            .horario_inicio = "05:00",
            .horario_fim = "09:00",
            .valor = 89.90,
            .atividades = {
                "Corrida",
                "Funcional",
                "Yoga",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-005",
            .nome = "Plano Executivo",
            .horario_inicio = "11:00",
            .horario_fim = "15:00",
            .valor = 179.90,
            .atividades = {
                "Musculacao",
                "Spinning",
                "Pilates",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-006",
            .nome = "Plano Noturno Plus",
            .horario_inicio = "19:00",
            .horario_fim = "23:00",
            .valor = 199.90,
            .atividades = {
                "Cross Training",
                "Natacao",
                "HIIT",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-007",
            .nome = "Plano Fim de Semana",
            .horario_inicio = "08:00",
            .horario_fim = "18:00",
            .valor = 119.90,
            .atividades = {
                "Musculacao",
                "Funcional",
                "Cardio",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-008",
            .nome = "Plano Estudante",
            .horario_inicio = "14:00",
            .horario_fim = "18:00",
            .valor = 79.90,
            .atividades = {
                "Musculacao",
                "Cardio",
            },
            .total_atividades = 2,
            .ativo = true,
        },
        {
            .id = "PLAN-009",
            .nome = "Plano Familia",
            .horario_inicio = "09:00",
            .horario_fim = "17:00",
            .valor = 249.90,
            .atividades = {
                "Natacao",
                "Hidroginastica",
                "Funcional",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-010",
            .nome = "Plano Senior",
            .horario_inicio = "08:00",
            .horario_fim = "12:00",
            .valor = 99.90,
            .atividades = {
                "Hidroginastica",
                "Alongamento",
                "Pilates",
            },
            .total_atividades = 3,
            .ativo = true,
        },
        {
            .id = "PLAN-011",
            .nome = "Plano Atleta",
            .horario_inicio = "06:00",
            .horario_fim = "22:00",
            .valor = 299.90,
            .atividades = {
                "Cross Training",
                "Musculacao",
                "HIIT",
                "Natacao",
                "Spinning",
            },
            .total_atividades = 5,
            .ativo = true,
        },
        {
            .id = "PLAN-012",
            .nome = "Plano Light",
            .horario_inicio = "10:00",
            .horario_fim = "14:00",
            .valor = 69.90,
            .atividades = {
                "Caminhada",
                "Alongamento",
            },
            .total_atividades = 2,
            .ativo = true,
        },
    };

    int total = sizeof(planos_iniciais) / sizeof(planos_iniciais[0]);

    for (int i = 0; i < total; i++)
    {
        lista_planos[i] = planos_iniciais[i];
    }

    int qtd_inativos = 1 + (rand() % 2); // 1 a 2
    int marcados = 0;
    while (marcados < qtd_inativos && marcados < total)
    {
        int idx = rand() % total;
        if (lista_planos[idx].ativo)
        {
            lista_planos[idx].ativo = false;
            marcados++;
        }
    }

    logEtapaGeracao("PLANOS", total);

    return total;
}

// Salva todos os planos ativos no arquivo binario
void salvarPlanos(struct plano lista_planos[], int total_planos)
{
    FILE *fp = fopen(TMP_FILE, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporario");
        return;
    }

    for (int i = 0; i < total_planos; i++)
    {
        if (lista_planos[i].ativo)
        {
            fwrite(&lista_planos[i], sizeof(struct plano), 1, fp);
        }
    }

    fclose(fp);
    remove(PLANOS_FILE);
    rename(TMP_FILE, PLANOS_FILE);
}

// Carrega todos os planos do arquivo binario
int carregarPlanos(struct plano lista_planos[])
{
    FILE *fp = fopen(PLANOS_FILE, "rb");
    if (!fp)
    {
        int lista_vazia = (lista_planos == NULL || lista_planos[0].id[0] == '\0');
        if (lista_vazia)
        {
            int total = preencherPlanosFicticios(lista_planos);
            salvarPlanos(lista_planos, total);
            return total;
        }
        return 0;
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

// Atualiza um plano especifico no arquivo
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

// Marca um plano como excluido (exclusao logica)
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
