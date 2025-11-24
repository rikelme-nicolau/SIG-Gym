#ifndef ARQUIVO_ALUNO_H
#define ARQUIVO_ALUNO_H

#include "cadastrarAluno.h" // contem struct aluno, lista_alunos[], total_alunos, MAX_ALUNOS

/* Rotinas de leitura/escrita dos alunos em arquivo binario.
   Mantem a assinatura simples para ser reaproveitada pelos outros fluxos. */

/* Percorre o vetor e grava apenas os alunos ativos no arquivo binario. */
void salvarAlunos(struct aluno lista_alunos[], int total_alunos);

/* Le o arquivo binario para o vetor informado e devolve o total carregado. */
int carregarAlunos(struct aluno lista_alunos[]);

/* Atualiza um aluno especifico no arquivo, mantendo os demais registros. */
void atualizarAlunoNoArquivo(struct aluno aluno);

/* Marca um aluno como inativo (exclusao logica) e regrava o arquivo. */
void excluirAluno(char *id);

#endif // ARQUIVO_ALUNO_H
