#ifndef CADASTRAR_EQUIPAMENTO_H
#define CADASTRAR_EQUIPAMENTO_H

#define MAX_EQUIPAMENTOS 1024
#define MAX_BUFFER 100

struct equipamento
{
    char id[12];
    char nome[MAX_BUFFER];
    char ultima_manutencao[11];  // formato dd/mm/aaaa
    char proxima_manutencao[11]; // calculada automaticamente (+60 dias)
    char categoria[MAX_BUFFER];  // grupo muscular
    int ativo;                   // 1 = ativo, 0 = excluído
};

extern struct equipamento lista_equipamentos[MAX_EQUIPAMENTOS];
extern int total_equipamentos;

void telaCadastrarEquipamento(void);

#endif
