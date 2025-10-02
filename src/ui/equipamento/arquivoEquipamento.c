#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"

#define EQUIPAMENTOS_FILE "equipamentos.txt"
#define TMP_FILE_EQUIP "equipamentos.tmp"

// Salva todos os equipamentos ativos no arquivo texto
void salvarEquipamentos(struct equipamento lista_equipamentos[], int total_equipamentos)
{
    FILE *fp = fopen(TMP_FILE_EQUIP, "wt");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_equipamentos; i++)
    {
        if (lista_equipamentos[i].ativo)
        {
            fprintf(fp, "%s;%s;%s;%s;%s;%d\n",
                    lista_equipamentos[i].id,
                    lista_equipamentos[i].nome,
                    lista_equipamentos[i].ultima_manutencao,
                    lista_equipamentos[i].proxima_manutencao,
                    lista_equipamentos[i].categoria,
                    lista_equipamentos[i].ativo);
        }
    }

    fclose(fp);
    remove(EQUIPAMENTOS_FILE);
    rename(TMP_FILE_EQUIP, EQUIPAMENTOS_FILE);
}

// Carrega todos os equipamentos do arquivo texto
int carregarEquipamentos(struct equipamento lista_equipamentos[])
{
    FILE *fp = fopen(EQUIPAMENTOS_FILE, "rt");
    if (!fp)
        return 0;

    int total = 0;
    while (!feof(fp) && total < MAX_EQUIPAMENTOS)
    {
        fscanf(fp, "%11[^;];%1023[^;];%11[^;];%11[^;];%1023[^;];%d\n",
               lista_equipamentos[total].id,
               lista_equipamentos[total].nome,
               lista_equipamentos[total].ultima_manutencao,
               lista_equipamentos[total].proxima_manutencao,
               lista_equipamentos[total].categoria,
               (int *)&lista_equipamentos[total].ativo);
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
