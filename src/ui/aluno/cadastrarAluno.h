#include <stdbool.h>

#ifndef CADASTRAR_ALUNO_H
#define CADASTRAR_ALUNO_H

/* Tela de cadastro de alunos. Tambem expoe o vetor global de alunos
   usado pelos demais arquivos do modulo. */
void telaCadastrarAluno(void);

#define MAX_BUFFER 1024
#define MAX_ALUNOS 1024

/* Estrutura persistida para cada aluno. Os tamanhos generosos evitam
   estouro de buffer em operacoes de leitura de texto. */
struct aluno
{
    char id[MAX_BUFFER];
    char nome[MAX_BUFFER];
    char idade[12];
    char cpf[22];
    char telefone[22];
    char endereco[MAX_BUFFER];
    char email[MAX_BUFFER];
    bool ativo;
    char plano_id[12];
};

extern struct aluno lista_alunos[MAX_ALUNOS];
extern int total_alunos;

#endif
