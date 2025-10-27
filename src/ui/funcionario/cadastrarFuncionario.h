#ifndef CADASTRAR_FUNCIONARIO_H
#define CADASTRAR_FUNCIONARIO_H

#include <stdbool.h>

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario
{
    char id[16];
    char nome[100];
    char nascimento[12]; // DD/MM/AAAA
    int idade;           // Calculada a partir do nascimento
    char cpf[15];
    char telefone[20];
    char endereco[150];
    char email[100];
    char cargo[20];
    bool ativo;
};

extern struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
extern int total_funcionarios;

void telaCadastrarFuncionario(void);

#endif
