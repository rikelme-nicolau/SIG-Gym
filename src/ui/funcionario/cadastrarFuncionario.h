#ifndef CADASTRAR_FUNCIONARIO_H
#define CADASTRAR_FUNCIONARIO_H

#include <stdbool.h>

#define MAX_BUFFER 1024
#define MAX_FUNCIONARIOS 1024

struct funcionario
{
    char id[12]; // ← era matricula
    char nome[MAX_BUFFER];
    char idade[12];
    char cpf[22];
    char endereco[MAX_BUFFER];
    char cargo[MAX_BUFFER];
    bool ativo;
};

// Declarações globais (somente referência)
extern struct funcionario lista_funcionarios[MAX_FUNCIONARIOS];
extern int total_funcionarios;

// Protótipo da função
void telaCadastrarFuncionario(void);

#endif
