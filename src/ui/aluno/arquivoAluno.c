#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadastrarAluno.h"
#include "arquivoAluno.h"

#define ALUNOS_FILE "alunos.txt"
#define TMP_FILE    "alunos.tmp"

// Salva todos os alunos ativos no arquivo texto
void salvarAlunos(struct aluno lista_alunos[], int total_alunos) {
    FILE *fp = fopen(TMP_FILE, "wt");
    if (!fp) {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        if (lista_alunos[i].ativo) {
            fprintf(fp, "%s;%s;%s;%s;%s;%s;%s;%d;%s\n",
                    lista_alunos[i].id,
                    lista_alunos[i].nome,
                    lista_alunos[i].idade,
                    lista_alunos[i].cpf,
                    lista_alunos[i].telefone,
                    lista_alunos[i].endereco,
                    lista_alunos[i].email,
                    lista_alunos[i].ativo,
                    lista_alunos[i].plano_id);
        }
    }

    fclose(fp);
    remove(ALUNOS_FILE);
    rename(TMP_FILE, ALUNOS_FILE);
}

// Carrega todos os alunos do arquivo texto
int carregarAlunos(struct aluno lista_alunos[]) {
    FILE *fp = fopen(ALUNOS_FILE, "rt");
    if (!fp) return 0;

    int total = 0;
    while (!feof(fp) && total < MAX_ALUNOS) {
        fscanf(fp, "%11[^;];%1023[^;];%11[^;];%21[^;];%21[^;];%1023[^;];%1023[^;];%d;%11[^\n]\n",
               lista_alunos[total].id,
               lista_alunos[total].nome,
               lista_alunos[total].idade,
               lista_alunos[total].cpf,
               lista_alunos[total].telefone,
               lista_alunos[total].endereco,
               lista_alunos[total].email,
               (int*)&lista_alunos[total].ativo,
               lista_alunos[total].plano_id);
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um aluno específico no arquivo
void atualizarAlunoNoArquivo(struct aluno aluno) {
    struct aluno alunos[MAX_ALUNOS];
    int total = carregarAlunos(alunos);

    for (int i = 0; i < total; i++) {
        if (strcmp(alunos[i].id, aluno.id) == 0) {
            alunos[i] = aluno;
            break;
        }
    }

    salvarAlunos(alunos, total);
}

// Marca um aluno como excluído (exclusão lógica)
void excluirAluno(char *id) {
    struct aluno alunos[MAX_ALUNOS];
    int total = carregarAlunos(alunos);

    for (int i = 0; i < total; i++) {
        if (strcmp(alunos[i].id, id) == 0) {
            alunos[i].ativo = false;
            break;
        }
    }

    salvarAlunos(alunos, total);
}
