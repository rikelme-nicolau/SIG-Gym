#ifndef ARQUIVO_EQUIPAMENTO_H
#define ARQUIVO_EQUIPAMENTO_H

#include "cadastrarEquipamento.h" // contém struct equipamento, lista_equipamentos[], total_equipamentos, MAX_EQUIPAMENTOS

// Funções de persistência do módulo equipamento

/**
 * @brief Salva todos os equipamentos ativos no arquivo
 * @param lista_equipamentos Vetor de equipamentos
 * @param total_equipamentos Quantidade total de equipamentos no vetor
 */
void salvarEquipamentos(struct equipamento lista_equipamentos[], int total_equipamentos);

/**
 * @brief Carrega todos os equipamentos do arquivo
 * @param lista_equipamentos Vetor que receberá os dados
 * @return Total de equipamentos carregados
 */
int carregarEquipamentos(struct equipamento lista_equipamentos[]);

/**
 * @brief Atualiza os dados de um equipamento específico no arquivo
 * @param equip Estrutura do equipamento com os dados atualizados
 */
void atualizarEquipamentoNoArquivo(struct equipamento equip);

/**
 * @brief Marca um equipamento como excluído (exclusão lógica) e atualiza o arquivo
 * @param id ID do equipamento a ser excluído
 */
void excluirEquipamento(char *id);

#endif // ARQUIVO_EQUIPAMENTO_H
