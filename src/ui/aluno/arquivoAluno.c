#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Incluído para usar o tipo 'bool'
#include "cadastrarAluno.h"
#include "arquivoAluno.h"

// ALTERAÇÃO: Mudamos a extensão para .dat para indicar que é um arquivo de dados binários.
#define ALUNOS_FILE "alunos.dat"
#define TMP_FILE    "alunos.tmp"

// Salva todos os alunos ativos no arquivo binário
void salvarAlunos(struct aluno lista_alunos[], int total_alunos) {
    // ALTERAÇÃO: Abrimos o arquivo em modo "wb" (write binary)
    FILE *fp = fopen(TMP_FILE, "wb");
    if (!fp) {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        // Salva apenas os alunos que estão marcados como ativos
        if (lista_alunos[i].ativo) {
            // ALTERAÇÃO: Usamos fwrite para escrever a struct inteira de uma vez.
            // Parâmetros: ponteiro para os dados, tamanho de cada item, quantidade de itens, ponteiro do arquivo.
            fwrite(&lista_alunos[i], sizeof(struct aluno), 1, fp);
        }
    }

    fclose(fp);
    remove(ALUNOS_FILE);
    rename(TMP_FILE, ALUNOS_FILE);
}

// Carrega todos os alunos do arquivo binário
int carregarAlunos(struct aluno lista_alunos[]) {
    // ALTERAÇÃO: Abrimos o arquivo em modo "rb" (read binary)
    FILE *fp = fopen(ALUNOS_FILE, "rb");
    if (!fp) {
        // Se o arquivo não existe, não há alunos para carregar.
        return 0;
    }

    int total = 0;

    // ALTERAÇÃO: Usamos fread para ler uma struct inteira do arquivo por vez.
    // O loop continua enquanto fread conseguir ler 1 item completo (sizeof(struct aluno)).
    while (total < MAX_ALUNOS && fread(&lista_alunos[total], sizeof(struct aluno), 1, fp) == 1) {
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um aluno específico no arquivo (lógica de alto nível inalterada)
// Esta função agora funcionará com o formato binário porque chama as
// funções carregarAlunos e salvarAlunos que foram refatoradas.
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

// Marca um aluno como excluído (lógica de alto nível inalterada)
// Esta função também funcionará com o formato binário pelas mesmas razões da anterior.
void excluirAluno(char *id) {
    struct aluno alunos[MAX_ALUNOS];
    int total = carregarAlunos(alunos);

    for (int i = 0; i < total; i++) {
        if (strcmp(alunos[i].id, id) == 0) {
            alunos[i].ativo = false; // Usa o tipo bool padrão
            break;
        }
    }

    salvarAlunos(alunos, total);
}