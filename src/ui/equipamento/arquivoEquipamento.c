#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"

// Agora usamos .dat pra indicar arquivo binário
#define EQUIPAMENTOS_FILE "equipamentos.dat"
#define TMP_FILE_EQUIP "equipamentos.tmp"

// Salva todos os equipamentos ativos no arquivo binário
void salvarEquipamentos(struct equipamento lista_equipamentos[], int total_equipamentos)
{
    FILE *fp = fopen(TMP_FILE_EQUIP, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            fwrite(&lista_equipamentos[i], sizeof(struct equipamento), 1, fp);
        }
    }

    fclose(fp);
    remove(EQUIPAMENTOS_FILE);
    rename(TMP_FILE_EQUIP, EQUIPAMENTOS_FILE);
}

// Carrega todos os equipamentos do arquivo binário
int carregarEquipamentos(struct equipamento lista_equipamentos[])
{
    FILE *fp = fopen(EQUIPAMENTOS_FILE, "rb");
    if (!fp)
        return 0;

    int total = 0;

    while (total < MAX_EQUIPAMENTOS && fread(&lista_equipamentos[total], sizeof(struct equipamento), 1, fp) == 1)
    {
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um equipamento específico no arquivo
void atualizarEquipamentoNoArquivo(struct equipamento equip)
{
    struct equipamento equipamentos[MAX_EQUIPAMENTOS];
    int total = carregarEquipamentos(equipamentos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(equipamentos[i].id, equip.id) == 0)
        {
            equipamentos[i] = equip;
            break;
        }
    }

    salvarEquipamentos(equipamentos, total);
}

// Marca um equipamento como excluído (exclusão lógica)
void excluirEquipamento(char *id)
{
    struct equipamento equipamentos[MAX_EQUIPAMENTOS];
    int total = carregarEquipamentos(equipamentos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(equipamentos[i].id, id) == 0)
        {
            equipamentos[i].ativo = false;
            break;
        }
    }

    salvarEquipamentos(equipamentos, total);
}
