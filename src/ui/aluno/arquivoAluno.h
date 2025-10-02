#ifndef ARQUIVO_ALUNO_H
#define ARQUIVO_ALUNO_H

#include "cadastrarAluno.h" // contém struct aluno, lista_alunos[], total_alunos, MAX_ALUNOS

// Funções de persistência do módulo aluno

/**
 * @brief Salva todos os alunos ativos no arquivo
 * @param lista_alunos Vetor de alunos
 * @param total_alunos Quantidade total de alunos no vetor
 */
void salvarAlunos(struct aluno lista_alunos[], int total_alunos);

/**
 * @brief Carrega todos os alunos do arquivo
 * @param lista_alunos Vetor que receberá os dados
 * @return Total de alunos carregados
 */
int carregarAlunos(struct aluno lista_alunos[]);

/**
 * @brief Atualiza os dados de um aluno específico no arquivo
 * @param aluno Estrutura do aluno com os dados atualizados
 */
void atualizarAlunoNoArquivo(struct aluno aluno);

/**
 * @brief Marca um aluno como excluído (exclusão lógica) e atualiza o arquivo
 * @param id ID do aluno a ser excluído
 */
void excluirAluno(char *id);

#endif // ARQUIVO_ALUNO_H
