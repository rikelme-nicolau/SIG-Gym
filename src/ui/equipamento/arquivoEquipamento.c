#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h"
#include "../utils/geradorDados.h"
#include "../utils/logGeracao.h"

#define EQUIPAMENTOS_FILE "equipamentos.dat"
#define TMP_FILE_EQUIP "equipamentos.tmp"
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
#define SEGUNDOS_DIA 86400

static time_t converterData(const char *data)
{
    int d, m, a;
    if (sscanf(data, "%2d/%2d/%4d", &d, &m, &a) != 3)
    {
        return (time_t)0;
    }

    struct tm tm_data = {0};
    tm_data.tm_mday = d;
    tm_data.tm_mon = m - 1;
    tm_data.tm_year = a - 1900;
    tm_data.tm_isdst = -1;

    return mktime(&tm_data);
}

static void garantirOrdemDatas(char *ultima, char *proxima)
{
    time_t t_ultima = converterData(ultima);
    time_t t_proxima = converterData(proxima);

    if (t_ultima == (time_t)0 || t_proxima == (time_t)0)
    {
        return;
    }

    if (difftime(t_ultima, t_proxima) >= 0)
    {
        time_t ajustada = t_ultima + (5 * SEGUNDOS_DIA);
        struct tm tm_ajustada = *localtime(&ajustada);
        strftime(proxima, 12, "%d/%m/%Y", &tm_ajustada);
    }
}

static int preencherEquipamentosFicticios(struct equipamento lista_equipamentos[])
{
    static const struct
    {
        const char *id;
        const char *nome;
        const char *categoria;
    } catalogo[] = {
        // Cardio (8)
        {"EQUIP-001", "Esteira Pro Runner", "Cardio"},
        {"EQUIP-002", "Esteira Ergometrica X1", "Cardio"},
        {"EQUIP-003", "Esteira Profissional T500", "Cardio"},
        {"EQUIP-004", "Bicicleta Spinning X3", "Cardio"},
        {"EQUIP-005", "Bike Ergometrica B200", "Cardio"},
        {"EQUIP-006", "Bicicleta Horizontal", "Cardio"},
        {"EQUIP-007", "Eliptico EL300", "Cardio"},
        {"EQUIP-008", "Transport EP450", "Cardio"},
        // Musculacao (10)
        {"EQUIP-009", "Leg Press 45", "Musculacao"},
        {"EQUIP-010", "Leg Press Horizontal", "Musculacao"},
        {"EQUIP-011", "Supino Reto", "Musculacao"},
        {"EQUIP-012", "Supino Inclinado", "Musculacao"},
        {"EQUIP-013", "Puxador Alto", "Musculacao"},
        {"EQUIP-014", "Puxador Baixo", "Musculacao"},
        {"EQUIP-015", "Cadeira Extensora", "Musculacao"},
        {"EQUIP-016", "Mesa Flexora", "Musculacao"},
        {"EQUIP-017", "Hack Machine", "Musculacao"},
        {"EQUIP-018", "Smith Machine", "Musculacao"},
        // Funcional (4)
        {"EQUIP-019", "TRX", "Funcional"},
        {"EQUIP-020", "Kettlebells (conjunto)", "Funcional"},
        {"EQUIP-021", "Medicine Balls", "Funcional"},
        {"EQUIP-022", "Corda Naval", "Funcional"},
        // Outros (3)
        {"EQUIP-023", "Estacao de Musculacao", "Outros"},
        {"EQUIP-024", "Rack de Agachamento", "Outros"},
        {"EQUIP-025", "Barra Fixa", "Outros"},
    };

    int total = 0;

    for (size_t i = 0; i < ARRAY_LENGTH(catalogo) && total < MAX_EQUIPAMENTOS; i++)
    {
        struct equipamento equip;
        memset(&equip, 0, sizeof(equip));

        strncpy(equip.id, catalogo[i].id, sizeof(equip.id) - 1);
        strncpy(equip.nome, catalogo[i].nome, sizeof(equip.nome) - 1);
        strncpy(equip.categoria, catalogo[i].categoria, sizeof(equip.categoria) - 1);

        int categoria_manutencao = rand() % 10;

        if (categoria_manutencao < 3)
        {
            gerarDataManutencaoAleatoria(equip.ultima_manutencao, -60, -30);
            gerarDataManutencaoAleatoria(equip.proxima_manutencao, -29, -1);
        }
        else if (categoria_manutencao < 5)
        {
            gerarDataManutencaoAleatoria(equip.ultima_manutencao, -30, -7);
            gerarDataManutencaoAleatoria(equip.proxima_manutencao, 1, 7);
        }
        else if (categoria_manutencao < 8)
        {
            gerarDataManutencaoAleatoria(equip.ultima_manutencao, -15, 0);
            gerarDataManutencaoAleatoria(equip.proxima_manutencao, 8, 30);
        }
        else
        {
            gerarDataManutencaoAleatoria(equip.ultima_manutencao, -30, 0);
            gerarDataManutencaoAleatoria(equip.proxima_manutencao, 31, 180);
        }

        garantirOrdemDatas(equip.ultima_manutencao, equip.proxima_manutencao);

        equip.ativo = true;
        lista_equipamentos[total++] = equip;
    }

    int qtd_inativos = 2 + (rand() % 2); // 2 a 3
    int marcados = 0;
    while (marcados < qtd_inativos && marcados < total)
    {
        int idx = rand() % total;
        if (lista_equipamentos[idx].ativo)
        {
            lista_equipamentos[idx].ativo = false;
            marcados++;
        }
    }

    logEtapaGeracao("EQUIPAMENTOS", total);

    return total;
}

static int gerarEquipamentosPadrao(struct equipamento lista_equipamentos[])
{
    int total = preencherEquipamentosFicticios(lista_equipamentos);
    salvarEquipamentos(lista_equipamentos, total);
    return total;
}

// Salva todos os equipamentos ativos no arquivo binario
void salvarEquipamentos(struct equipamento lista_equipamentos[], int total_equipamentos)
{
    FILE *fp = fopen(TMP_FILE_EQUIP, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporario");
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

// Carrega todos os equipamentos do arquivo binario
int carregarEquipamentos(struct equipamento lista_equipamentos[])
{
    FILE *fp = fopen(EQUIPAMENTOS_FILE, "rb");
    if (!fp)
    {
        printf("Arquivo de equipamentos nao encontrado. Gerando dados ficticios...\n");
        return gerarEquipamentosPadrao(lista_equipamentos);
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
        printf("Arquivo de equipamentos vazio. Gerando dados ficticios...\n");
        return gerarEquipamentosPadrao(lista_equipamentos);
    }

    int total = 0;

    while (total < MAX_EQUIPAMENTOS && fread(&lista_equipamentos[total], sizeof(struct equipamento), 1, fp) == 1)
    {
        total++;
    }

    fclose(fp);

    if (total == 0)
    {
        printf("Nenhum equipamento valido encontrado. Gerando dados ficticios...\n");
        return gerarEquipamentosPadrao(lista_equipamentos);
    }

    return total;
}

// Atualiza um equipamento especifico no arquivo
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

// Marca um equipamento como excluido (exclusao logica)
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
