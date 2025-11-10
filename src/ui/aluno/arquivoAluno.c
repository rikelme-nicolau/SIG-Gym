#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Incluído para usar o tipo 'bool'
#include "cadastrarAluno.h"
#include "arquivoAluno.h"

// ALTERAÇÃO: Mudamos a extensão para .dat para indicar que é um arquivo de dados binários.
#define ALUNOS_FILE "alunos.dat"
#define TMP_FILE    "alunos.tmp"

static int preencherAlunosFicticios(struct aluno lista_alunos[])
{
    static const struct aluno alunos_iniciais[] = {
        {
            .id = "ALU-0001",
            .nome = "João Silva",
            .idade = "29",
            .cpf = "123.456.789-00",
            .telefone = "(11) 99876-5432",
            .endereco = "Rua das Palmeiras, 120 - Centro",
            .email = "joao.silva@example.com",
            .ativo = true,
            .plano_id = "PLAN-001",
        },
        {
            .id = "ALU-0002",
            .nome = "Maria Souza",
            .idade = "34",
            .cpf = "987.654.321-00",
            .telefone = "(11) 91234-5678",
            .endereco = "Av. Brasil, 45 - Jardim Europa",
            .email = "maria.souza@example.com",
            .ativo = true,
            .plano_id = "PLAN-002",
        },
        {
            .id = "ALU-0003",
            .nome = "Carlos Pereira",
            .idade = "41",
            .cpf = "321.654.987-00",
            .telefone = "(11) 97777-1122",
            .endereco = "Rua do Sol, 88 - Vila Nova",
            .email = "carlos.pereira@example.com",
            .ativo = true,
            .plano_id = "PLAN-003",
        },
    };

    int total = sizeof(alunos_iniciais) / sizeof(alunos_iniciais[0]);

    for (int i = 0; i < total; i++)
    {
        lista_alunos[i] = alunos_iniciais[i];
    }

    return total;
}

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
        int total = preencherAlunosFicticios(lista_alunos);
        salvarAlunos(lista_alunos, total);
        return total;
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