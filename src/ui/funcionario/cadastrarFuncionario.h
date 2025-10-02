#include <stdbool.h>

#ifndef CADASTRAR_FUNCIONARIO_H
#define CADASTRAR_FUNCIONARIO_H

void telaCadastrarFuncionario(void);

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario
{
    char id[MAX_BUFFER];
    char nome[MAX_BUFFER];
    char idade[12];
    char cpf[22];
    char endereco[MAX_BUFFER];
    char cargo[MAX_BUFFER];
    bool ativo;
};

extern struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
extern int total_funcionarios;

#endif
