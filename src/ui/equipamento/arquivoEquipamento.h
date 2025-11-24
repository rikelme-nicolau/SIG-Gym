#ifndef ARQUIVO_EQUIPAMENTO_H
#define ARQUIVO_EQUIPAMENTO_H

#include "cadastrarEquipamento.h" // contem struct equipamento, lista_equipamentos[], total_equipamentos, MAX_EQUIPAMENTOS

/* Funcoes de persistencia do modulo de equipamentos. */

/* Grava os equipamentos ativos no arquivo binario. */
void salvarEquipamentos(struct equipamento lista_equipamentos[], int total_equipamentos);

/* Le o arquivo para o vetor informado e devolve o total carregado. */
int carregarEquipamentos(struct equipamento lista_equipamentos[]);

/* Atualiza um equipamento especifico no arquivo, mantendo os demais. */
void atualizarEquipamentoNoArquivo(struct equipamento equip);

/* Marca um equipamento como inativo (exclusao logica) e regrava o arquivo. */
void excluirEquipamento(char *id);

#endif // ARQUIVO_EQUIPAMENTO_H
