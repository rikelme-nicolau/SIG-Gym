#ifndef CADASTRARPLANO_H
#define CADASTRARPLANO_H

#include <stdbool.h>

void telaCadastrarPlano(void);

#define MAX_PLANOS 50
#define MAX_BUFFER 1024
#define MAX_ATIVIDADES 5

struct plano {
    char id[12];
    char nome[MAX_BUFFER];
    char horario[MAX_BUFFER];
    char atividades[MAX_ATIVIDADES][MAX_BUFFER];
    int total_atividades;
    bool ativo; 
};

extern struct plano lista_planos[MAX_PLANOS];
extern int total_planos;

#endif
