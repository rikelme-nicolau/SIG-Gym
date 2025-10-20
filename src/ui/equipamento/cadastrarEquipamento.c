#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "limparTela.h"
#include "cadastrarEquipamento.h"
#include "arquivoEquipamento.h" // Persistência



#define MAX_BUFFER 1024

struct equipamento lista_equipamentos[MAX_EQUIPAMENTOS];
int total_equipamentos = 0;

// Função auxiliar para calcular data + 60 dias
void calcularProximaManutencao(const char *data, char *saida)
{
    struct tm tm_data = {0};
    if (strptime(data, "%d/%m/%Y", &tm_data) == NULL)
    {
        strcpy(saida, "Inválida");
        return;
    }

    tm_data.tm_mday += 60;
    mktime(&tm_data);

    strftime(saida, 11, "%d/%m/%Y", &tm_data);
}

void telaCadastrarEquipamento(void)
{
    limparTela();
    printf("=========================================================================\n");
    printf("===                      CADASTRAR EQUIPAMENTO                        ===\n");
    printf("=========================================================================\n");

    if (total_equipamentos >= MAX_EQUIPAMENTOS)
    {
        printf("=== LIMITE MÁXIMO DE EQUIPAMENTOS ALCANÇADO ===\n");
        getchar();
        limparTela();
        return;
    }

    struct equipamento novo;

    snprintf(novo.id, sizeof(novo.id), "%d", total_equipamentos + 1);

    // Nome do equipamento
    printf("Nome do equipamento: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    // Última manutenção
    printf("Data da última manutenção (dd/mm/aaaa): ");
    fgets(novo.ultima_manutencao, sizeof(novo.ultima_manutencao), stdin);
    novo.ultima_manutencao[strcspn(novo.ultima_manutencao, "\n")] = '\0';

    // Próxima manutenção
    calcularProximaManutencao(novo.ultima_manutencao, novo.proxima_manutencao);

    // Limpar buffer antes da categoria
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // Categoria
    printf("Categoria (grupo muscular principal): ");
    fgets(novo.categoria, sizeof(novo.categoria), stdin);
    novo.categoria[strcspn(novo.categoria, "\n")] = '\0';

    // Marcar ativo e adicionar
    novo.ativo = true;
    lista_equipamentos[total_equipamentos++] = novo;

    // Salvar automaticamente usando o arquivo de persistência
    salvarEquipamentos(lista_equipamentos, total_equipamentos);

    // Mensagem de sucesso
    printf("\n=========================================================================\n");
    printf("===                  EQUIPAMENTO CADASTRADO COM SUCESSO               ===\n");
    printf("=========================================================================\n");
    printf(">>> Pressione <ENTER> para continuar...");
    getchar();
    limparTela();
}
