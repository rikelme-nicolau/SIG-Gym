#ifndef ARQUIVO_FUNCIONARIO_H
#define ARQUIVO_FUNCIONARIO_H

#include "cadastrarFuncionario.h" // contém struct funcionario, lista_funcionarios[], total_funcionarios, MAX_FUNCIONARIOS

// Funções de persistência do módulo funcionário

/**
 * @brief Salva todos os funcionários ativos no arquivo
 * @param lista_funcionarios Vetor de funcionários
 * @param total_funcionarios Quantidade total de funcionários no vetor
 */
void salvarFuncionarios(struct funcionario lista_funcionarios[], int total_funcionarios);

/**
 * @brief Carrega todos os funcionários do arquivo
 * @param lista_funcionarios Vetor que receberá os dados
 * @return Total de funcionários carregados
 */
int carregarFuncionarios(struct funcionario lista_funcionarios[]);

/**
 * @brief Atualiza os dados de um funcionário específico no arquivo
 * @param func Estrutura do funcionário com os dados atualizados
 */
void atualizarFuncionarioNoArquivo(struct funcionario func);

/**
 * @brief Marca um funcionário como excluído (exclusão lógica) e atualiza o arquivo
 * @param id ID do funcionário a ser excluído
 */
void excluirFuncionario(char *id);

#endif // ARQUIVO_FUNCIONARIO_H
