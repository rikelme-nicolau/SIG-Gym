#ifndef CADASTRAR_EQUIPAMENTO_H
#define CADASTRAR_EQUIPAMENTO_H
#include <stdbool.h>
void telaCadastrarEquipamento(void);

#define MAX_BUFFER 1024
#define MAX_EQUIPAMENTOS 1024

void calcularProximaManutencao(const char*, char*);

struct equipamento
{
    char id[12];
    char nome[MAX_BUFFER];
    char ultima_manutencao[12];
    char proxima_manutencao[12];
    char categoria[MAX_BUFFER];
    bool ativo;
};

extern struct equipamento lista_equipamentos[MAX_EQUIPAMENTOS];
extern int total_equipamentos;

#endif
