#ifndef ARQUIVO_PLANO_H
#define ARQUIVO_PLANO_H

#include "cadastrarPlano.h"

// Funções de persistência do módulo plano
void salvarPlanos(struct plano lista_planos[], int total_planos);
int carregarPlanos(struct plano lista_planos[]);
void atualizarPlanoNoArquivo(struct plano plano);
void excluirPlano(char *id);

#endif // ARQUIVO_PLANO_H
